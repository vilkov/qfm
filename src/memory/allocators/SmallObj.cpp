#include "SmallObj.h"

#include <cassert>
#include <climits>
#include <vector>
#include <bitset>

#define STLSOFT_FORCE_ATORS_RETURN_NULL
#include <stlsoft/memory/malloc_allocator.hpp>

/* For memory manipulations */
#include <stdlib.h>
#include <string.h>

#ifdef DO_EXTRA_LOKI_TESTS
    #include <iostream>
#endif

namespace Loki
{
    /** @struct Chunk
        @ingroup SmallObjectGroupInternal
     Contains info about each allocated Chunk - which is a collection of
     contiguous blocks.  Each block is the same size, as specified by the
     FixedAllocator.  The number of blocks in a Chunk depends upon page size.
     This is a POD-style struct with value-semantics.  All functions and data
     are private so that they can not be changed by anything other than the
     FixedAllocator which owns the Chunk.

     @par Minimal Interface
     For the sake of runtime efficiency, no constructor, destructor, or
     copy-assignment operator is defined. The inline functions made by the
     compiler should be sufficient, and perhaps faster than hand-crafted
     functions.  The lack of these functions allows vector to create and copy
     Chunks as needed without overhead.  The Init and Release functions do
     what the default constructor and destructor would do.  A Chunk is not in
     a usable state after it is constructed and before calling Init.  Nor is
     a Chunk usable after Release is called, but before the destructor.

     @par Efficiency
     Down near the lowest level of the allocator, runtime efficiencies trump
     almost all other considerations.  Each function does the minimum required
     of it.  All functions should execute in constant time to prevent higher-
     level code from unwittingly using a version of Shlemiel the Painter's
     Algorithm.

     @par Stealth Indexes
     The first char of each empty block contains the index of the next empty
     block.  These stealth indexes form a singly-linked list within the blocks.
     A Chunk is corrupt if this singly-linked list has a loop or is shorter
     than blocksAvailable_.  Much of the allocator's time and space efficiency
     comes from how these stealth indexes are implemented.
     */
    struct Chunk
    {
    	typedef unsigned char  value_type;
    	typedef unsigned short size_type;

        /** Initializes a just-constructed Chunk.
         @param blockSize Number of bytes per block.
         @param blocks Number of blocks per Chunk.
         @return True for success, false for failure.
         */
        bool init(std::size_t blockSize, size_type blocks)
        {
            assert(blockSize > 0);
            assert(blocks > 0);
            // Overflow check
            assert(blockSize * blocks / blockSize == blocks);

            // malloc can't throw, so its only way to indicate an error is to return
            // a NULL pointer, so we have to check for that.
            data = static_cast<value_type *>(malloc(blockSize * blocks));
            if (NULL == data)
            	return false;

            reset(blockSize, blocks);
            return true;
        }

        /** Allocate a block within the Chunk.  Complexity is always O(1), and
         this will never throw.  Does not actually "allocate" by calling
         malloc, new, or any other function, but merely adjusts some internal
         indexes to indicate an already allocated block is no longer available.
         @return Pointer to block within Chunk.
         */
        void *allocate(std::size_t blockSize)
        {
            if (isFilled())
            	return NULL;

            assert((firstAvailableBlock * blockSize) / blockSize == firstAvailableBlock);
            value_type *pResult = data + (firstAvailableBlock * blockSize);
            firstAvailableBlock = *((size_type *)pResult);
            --blocksAvailable;

            return pResult;
        }

        /** Deallocate a block within the Chunk. Complexity is always O(1), and
         this will never throw.  For efficiency, this assumes the address is
         within the block and aligned along the correct byte boundary.  An
         assertion checks the alignment, and a call to HasBlock is done from
         within VicinityFind.  Does not actually "deallocate" by calling free,
         delete, or other function, but merely adjusts some internal indexes to
         indicate a block is now available.
         */
        void deallocate(void *p, std::size_t blockSize)
        {
            assert(p >= data);

            value_type *toRelease = static_cast<value_type *>(p);
            // Alignment check
            assert((toRelease - data) % blockSize == 0);
            size_type index = static_cast<size_type>((toRelease - data) / blockSize);

        #if defined(DEBUG) || defined(_DEBUG)
            // Check if block was already deleted.  Attempting to delete the same
            // block more than once causes Chunk's linked-list of stealth indexes to
            // become corrupt.  And causes count of blocksAvailable_ to be wrong.
            if ( 0 < blocksAvailable )
                assert(firstAvailableBlock != index);
        #endif

            *((size_type *)toRelease) = firstAvailableBlock;
            firstAvailableBlock = index;
            // Truncation check
            assert(firstAvailableBlock == (toRelease - data) / blockSize);

            ++blocksAvailable;
        }

        /** Resets the Chunk back to pristine values. The available count is
         set back to zero, and the first available index is set to the zeroth
         block.  The stealth indexes inside each block are set to point to the
         next block. This assumes the Chunk's data was already using Init.
         */
        void reset(std::size_t blockSize, size_type blocks)
        {
            assert(blockSize > 0);
            assert(blocks > 0);
            // Overflow check
            assert((blockSize * blocks) / blockSize == blocks);

            firstAvailableBlock = 0;
            blocksAvailable = blocks;

            size_type i = 0;
            for (value_type *p = data; i != blocks; p += blockSize)
                *((size_type *)p) = ++i;
        }

        /// Releases the allocated block of memory.
        void release()
        {
            assert(NULL != data);
            free(static_cast<void *>(data));
        }

        /** Determines if the Chunk has been corrupted.
         @param numBlocks Total # of blocks in the Chunk.
         @param blockSize # of bytes in each block.
         @param checkIndexes True if caller wants to check indexes of available
          blocks for corruption.  If false, then caller wants to skip some
          tests tests just to run faster.  (Debug version does more checks, but
          release version runs faster.)
         @return True if Chunk is corrupt.
         */
        bool isCorrupt(size_type numBlocks, std::size_t blockSize, bool checkIndexes) const
        {
            if (numBlocks < blocksAvailable)
            {
                // Contents at this Chunk corrupted.  This might mean something has
                // overwritten memory owned by the Chunks container.
                assert(false);
                return true;
            }

            if (isFilled())
                // Useless to do further corruption checks if all blocks allocated.
                return false;

            size_type index = firstAvailableBlock;
            if (numBlocks <= index)
            {
                // Contents at this Chunk corrupted.  This might mean something has
                // overwritten memory owned by the Chunks container.
                assert(false);
                return true;
            }

            if (!checkIndexes)
                // Caller chose to skip more complex corruption tests.
                return false;

            /* If the bit at index was set in foundBlocks, then the stealth index was
             found on the linked-list.
             */
            std::bitset<UCHAR_MAX> foundBlocks;
            value_type *nextBlock = NULL;

            /* The loop goes along singly linked-list of stealth indexes and makes sure
             that each index is within bounds (0 <= index < numBlocks) and that the
             index was not already found while traversing the linked-list.  The linked-
             list should have exactly blocksAvailable_ nodes, so the for loop will not
             check more than blocksAvailable_.  This loop can't check inside allocated
             blocks for corruption since such blocks are not within the linked-list.
             Contents of allocated blocks are not changed by Chunk.

             Here are the types of corrupted link-lists which can be verified.  The
             corrupt index is shown with asterisks in each example.

             Type 1: Index is too big.
              numBlocks == 64
              blocksAvailable_ == 7
              firstAvailableBlock_ -> 17 -> 29 -> *101*
              There should be no indexes which are equal to or larger than the total
              number of blocks.  Such an index would refer to a block beyond the
              Chunk's allocated domain.

             Type 2: Index is repeated.
              numBlocks == 64
              blocksAvailable_ == 5
              firstAvailableBlock_ -> 17 -> 29 -> 53 -> *17* -> 29 -> 53 ...
              No index should be repeated within the linked-list since that would
              indicate the presence of a loop in the linked-list.
             */
            for (size_type cc = 0; ;)
            {
                nextBlock = data + (index * blockSize);
                foundBlocks.set(index, true);
                ++cc;

                if (cc >= blocksAvailable)
                    // Successfully counted off number of nodes in linked-list.
                    break;

                index = *((size_type *)nextBlock);
                if (numBlocks <= index)
                {
                    /* This catches Type 1 corruptions as shown in above comments.
                     This implies that a block was corrupted due to a stray pointer
                     or an operation on a nearby block overran the size of the block.
                     */
                    assert(false);
                    return true;
                }

                if (foundBlocks.test(index))
                {
                    /* This catches Type 2 corruptions as shown in above comments.
                     This implies that a block was corrupted due to a stray pointer
                     or an operation on a nearby block overran the size of the block.
                     Or perhaps the program tried to delete a block more than once.
                     */
                    assert(false);
                    return true;
                }
            }

            if (foundBlocks.count() != blocksAvailable)
            {
                /* This implies that the singly-linked-list of stealth indexes was
                 corrupted.  Ideally, this should have been detected within the loop.
                 */
                assert(false);
                return true;
            }

            return false;
        }

        /** Determines if block is available.
         @param p Address of block managed by Chunk.
         @param numBlocks Total # of blocks in the Chunk.
         @param blockSize # of bytes in each block.
         @return True if block is available, else false if allocated.
         */
        bool isBlockAvailable(void *p, size_type numBlocks, std::size_t blockSize) const
        {
            (void) numBlocks;

            if (isFilled())
                return false;

            // Alignment check
            assert((static_cast<value_type *>(p) - data) % blockSize == 0);
            size_type blockIndex = static_cast<size_type>((static_cast<value_type *>(p) - data) / blockSize);

            size_type index = firstAvailableBlock;
            assert(numBlocks > index);
            if (index == blockIndex)
                return true;

            /* If the bit at index was set in foundBlocks, then the stealth index was
             found on the linked-list.
             */
            std::bitset<UCHAR_MAX> foundBlocks;
            value_type *nextBlock = NULL;
            for (size_type cc = 0; ;)
            {
                nextBlock = data + (index * blockSize);
                foundBlocks.set(index, true);
                ++cc;

                if (cc >= blocksAvailable)
                    // Successfully counted off number of nodes in linked-list.
                    break;

                index = *((size_type *)nextBlock);
                if (index == blockIndex)
                    return true;

                assert(numBlocks > index);
                assert(!foundBlocks.test(index));
            }

            return false;
        }

        /// Returns true if block at address P is inside this Chunk.
        inline bool hasBlock(void *p, std::size_t chunkLength) const
        {
            value_type *pc = static_cast<value_type *>(p);
            return (data <= pc) && (pc < data + chunkLength);
        }

        inline bool hasAvailable(size_type numBlocks) const { return blocksAvailable == numBlocks; }

        inline bool isFilled() const { return 0 == blocksAvailable; }

        /// Pointer to array of allocated blocks.
        value_type *data;
        /// Index of first empty block.
        size_type firstAvailableBlock;
        /// Count of empty blocks.
        size_type blocksAvailable;
    };

    /** @class FixedAllocator
        @ingroup SmallObjectGroupInternal
     Offers services for allocating fixed-sized objects.  It has a container
     of "containers" of fixed-size blocks.  The outer container has all the
     Chunks.  The inner container is a Chunk which owns some blocks.

     @par Class Level Invariants
     - There is always either zero or one Chunk which is empty.
     - If this has no empty Chunk, then emptyChunk_ is NULL.
     - If this has an empty Chunk, then emptyChunk_ points to it.
     - If the Chunk container is empty, then deallocChunk_ and allocChunk_
       are NULL.
     - If the Chunk container is not-empty, then deallocChunk_ and allocChunk_
       are either NULL or point to Chunks within the container.
     - allocChunk_ will often point to the last Chunk in the container since
       it was likely allocated most recently, and therefore likely to have an
       available block.
     */
    class FixedAllocator
    {
    public:
    	typedef Chunk::size_type size_type;

    	/// Create a FixedAllocator which manages blocks of 'blockSize' size.
        FixedAllocator() :
        	m_blockSize(0),
        	m_numBlocks(0),
        	m_allocChunk(NULL),
        	m_deallocChunk(NULL),
        	m_emptyChunk(NULL)
		{}

        /// Destroy the FixedAllocator and release all its Chunks.
        ~FixedAllocator()
        {
		#ifdef DO_EXTRA_LOKI_TESTS
			trimEmptyChunk();
			assert( m_chunks.empty() && "Memory leak detected!" );
		#endif
			for (ChunkIter i = m_chunks.begin(), end = m_chunks.end(); i != end; ++i)
			   i->release();
        }

        /// Initializes a FixedAllocator by calculating # of blocks per Chunk.
        void initialize(std::size_t blockSize, std::size_t pageSize)
        {
            assert(blockSize > 0);
            assert(pageSize >= blockSize);
            m_blockSize = blockSize;

            std::size_t blocks = pageSize / blockSize;
            if (blocks > MaxObjectsPerChunk)
            	blocks = MaxObjectsPerChunk;
            else
            	if (blocks < MinObjectsPerChunk)
            		blocks = MinObjectsPerChunk;

            m_numBlocks = static_cast<size_type>(blocks);
            assert(m_numBlocks == blocks);
        }

        /** Returns pointer to allocated memory block of fixed size - or NULL
         if it failed to allocate.
         */
        void *allocate()
        {
            // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
            assert((NULL == m_emptyChunk) || (m_emptyChunk->hasAvailable(m_numBlocks)));
            assert(countEmptyChunks() < 2);

            if (NULL == m_allocChunk || m_allocChunk->isFilled())
            {
                if (NULL != m_emptyChunk)
                {
                    m_allocChunk = m_emptyChunk;
                    m_emptyChunk = NULL;
                }
                else
                    for (ChunkIter i = m_chunks.begin(), end = m_chunks.end(); ; ++i)
                    {
                        if (end == i)
                        {
                            if (!makeNewChunk())
                                return NULL;
                            break;
                        }

						if (!i->isFilled())
						{
							m_allocChunk = &*i;
							break;
						}
                    }
            }
            else
            	if (m_allocChunk == m_emptyChunk)
                // detach emptyChunk_ from allocChunk_, because after
                // calling allocChunk_->Allocate(blockSize_); the chunk
                // is no longer empty.
            		m_emptyChunk = NULL;

            assert(m_allocChunk != NULL);
            assert(!m_allocChunk->isFilled());

            void *place = m_allocChunk->allocate(m_blockSize);

            // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
            assert((NULL == m_emptyChunk) || (m_emptyChunk->hasAvailable( m_numBlocks)));
            assert(countEmptyChunks() < 2);
        #ifdef LOKI_CHECK_FOR_CORRUPTION
            assert(!m_allocChunk->isCorrupt(m_numBlocks, m_blockSize, true));
        #endif

            return place;
        }

        /** Deallocate a memory block previously allocated with Allocate.  If
         the block is not owned by this FixedAllocator, it returns false so
         that SmallObjAllocator can call the default deallocator.  If the
         block was found, this returns true.
         */
        void deallocate(void *p, Chunk *hint)
        {
            assert(!m_chunks.empty());
            assert(&m_chunks.front() <= m_deallocChunk);
            assert(&m_chunks.back() >= m_deallocChunk);
            assert(&m_chunks.front() <= m_allocChunk);
            assert(&m_chunks.back() >= m_allocChunk);
            assert(countEmptyChunks() < 2);

            Chunk * foundChunk = (NULL == hint) ? vicinityFind(p) : hint;
            assert(NULL != foundChunk);
            assert(foundChunk->hasBlock(p, m_numBlocks * m_blockSize));

        #ifdef LOKI_CHECK_FOR_CORRUPTION
            assert(!foundChunk->isCorrupt(m_numBlocks, m_blockSize, true));
            assert(!foundChunk->isBlockAvailable(p, m_numBlocks, m_blockSize));
        #endif
            m_deallocChunk = foundChunk;
            doDeallocate(p);
            assert(countEmptyChunks() < 2);
        }

        /// Returns block size with which the FixedAllocator was initialized.
        inline std::size_t blockSize() const { return m_blockSize; }

        /** Releases the memory used by the empty Chunk.  This will take
         constant time under any situation.
         @return True if empty chunk found and released, false if none empty.
         */
        bool trimEmptyChunk()
        {
            // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
            assert((NULL == m_emptyChunk) || (m_emptyChunk->hasAvailable(m_numBlocks)));
            if (NULL == m_emptyChunk)
            	return false;

            // If emptyChunk_ points to valid Chunk, then chunk list is not empty.
            assert(!m_chunks.empty());
            // And there should be exactly 1 empty Chunk.
            assert(1 == countEmptyChunks());

            Chunk *lastChunk = &m_chunks.back();
            if (lastChunk != m_emptyChunk)
                std::swap(*m_emptyChunk, *lastChunk);

            assert(lastChunk->hasAvailable(m_numBlocks));
            lastChunk->release();
            m_chunks.pop_back();

            if (m_chunks.empty())
            {
                m_allocChunk = NULL;
                m_deallocChunk = NULL;
            }
            else
            {
                if (m_deallocChunk == m_emptyChunk)
                {
                    m_deallocChunk = &m_chunks.front();
                    assert(m_deallocChunk->blocksAvailable < m_numBlocks);
                }

                if (m_allocChunk == m_emptyChunk)
                {
                    m_allocChunk = &m_chunks.back();
                    assert(m_allocChunk->blocksAvailable < m_numBlocks);
                }
            }

            m_emptyChunk = NULL;
            assert(0 == countEmptyChunks());

            return true;
        }

        /** Releases unused spots from ChunkList.  This takes constant time
         with respect to # of Chunks, but actual time depends on underlying
         memory allocator.
         @return False if no unused spots, true if some found and released.
         */
        bool trimChunkList()
        {
			assert(m_chunks.empty() && NULL == m_allocChunk);
			assert(m_chunks.empty() && NULL == m_deallocChunk);

            if (m_chunks.size() == m_chunks.capacity())
                return false;

            // Use the "make-a-temp-and-swap" trick to remove excess capacity.
            Chunks(m_chunks).swap(m_chunks);

            return true;
        }

        /** Returns count of empty Chunks held by this allocator.  Complexity
         is O(C) where C is the total number of Chunks - empty or used.
         */
        std::size_t countEmptyChunks() const
        {
        #ifdef DO_EXTRA_LOKI_TESTS
            // This code is only used for specialized tests of the allocator.
            // It is #ifdef-ed so that its O(C) complexity does not overwhelm the
            // functions which call it.
            std::size_t count = 0;
            for (ChunkCIter it = m_chunks.begin(), end = m_chunks.end(); it != end; ++it)
                if ((*it).hasAvailable(m_numBlocks))
                    ++count;

            return count;
        #else
            return (NULL == m_emptyChunk) ? 0 : 1;
        #endif
        }

        /** Determines if FixedAllocator is corrupt.  Checks data members to
         see if any have erroneous values, or violate class invariants.  It
         also checks if any Chunk is corrupt.  Complexity is O(C) where C is
         the number of Chunks.  If any data is corrupt, this will return true
         in release mode, or assert in debug mode.
         */
        bool isCorrupt() const
        {
            ChunkCIter start = m_chunks.begin();
            ChunkCIter last = m_chunks.end();
            const size_t emptyChunkCount = countEmptyChunks();

            if (m_chunks.empty())
            {
                if ( start != last )
                {
                    assert( false );
                    return true;
                }
                if ( 0 < emptyChunkCount )
                {
                    assert( false );
                    return true;
                }
                if ( NULL != m_deallocChunk )
                {
                    assert( false );
                    return true;
                }
                if ( NULL != m_allocChunk )
                {
                    assert( false );
                    return true;
                }
                if ( NULL != m_emptyChunk )
                {
                    assert( false );
                    return true;
                }
            }
            else
            {
                const Chunk * front = &m_chunks.front();
                const Chunk * back  = &m_chunks.back();
                if ( start >= last )
                {
                    assert( false );
                    return true;
                }
                if ( back < m_deallocChunk )
                {
                    assert( false );
                    return true;
                }
                if ( back < m_allocChunk )
                {
                    assert( false );
                    return true;
                }
                if ( front > m_deallocChunk )
                {
                    assert( false );
                    return true;
                }
                if ( front > m_allocChunk )
                {
                    assert( false );
                    return true;
                }

                switch ( emptyChunkCount )
                {
                    case 0:
                        if ( m_emptyChunk != NULL )
                        {
                            assert( false );
                            return true;
                        }
                        break;
                    case 1:
                        if ( m_emptyChunk == NULL )
                        {
                            assert( false );
                            return true;
                        }
                        if ( back < m_emptyChunk )
                        {
                            assert( false );
                            return true;
                        }
                        if ( front > m_emptyChunk )
                        {
                            assert( false );
                            return true;
                        }
                        if ( !m_emptyChunk->hasAvailable( m_numBlocks ) )
                        {
                            // This may imply somebody tried to delete a block twice.
                            assert( false );
                            return true;
                        }
                        break;
                    default:
                        assert( false );
                        return true;
                }
                for ( ChunkCIter it( start ); it != last; ++it )
                {
                    const Chunk & chunk = *it;
                    if ( chunk.isCorrupt( m_numBlocks, m_blockSize, true ) )
                        return true;
                }
            }

            return false;
        }

        /** Returns true if the block at address p is within a Chunk owned by
         this FixedAllocator.  Complexity is O(C) where C is the total number
         of Chunks - empty or used.
         */
        const Chunk *hasBlock(void *p) const
        {
            const std::size_t chunkLength = m_numBlocks * m_blockSize;

            for (ChunkCIter it = m_chunks.begin(), end = m_chunks.end(); it != end; ++it )
                if ((*it).hasBlock(p, chunkLength))
                    return &(*it);

            return NULL;
        }
        inline Chunk *hasBlock(void *p)
        {
            return const_cast<Chunk *>(const_cast<const FixedAllocator *>(this)->hasBlock(p));
        }

    private:
        /// Not implemented.
        FixedAllocator(const FixedAllocator&);
        /// Not implemented.
        FixedAllocator& operator=(const FixedAllocator&);

    private:
        /** Deallocates the block at address p, and then handles the internal
         bookkeeping needed to maintain class invariants.  This assumes that
         deallocChunk_ points to the correct chunk.
         */
        void doDeallocate(void *p)
        {
            // Show that deallocChunk_ really owns the block at address p.
            assert(m_deallocChunk->hasBlock(p, m_numBlocks * m_blockSize));
            // Either of the next two assertions may fail if somebody tries to
            // delete the same block twice.
            assert(m_emptyChunk != m_deallocChunk);
            assert(!m_deallocChunk->hasAvailable(m_numBlocks));
            // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
            assert((NULL == m_emptyChunk) || (m_emptyChunk->hasAvailable(m_numBlocks)));

            // call into the chunk, will adjust the inner list but won't release memory
            m_deallocChunk->deallocate(p, m_blockSize);

            if (m_deallocChunk->hasAvailable(m_numBlocks))
            {
                assert(m_emptyChunk != m_deallocChunk);
                // deallocChunk_ is empty, but a Chunk is only released if there are 2
                // empty chunks.  Since emptyChunk_ may only point to a previously
                // cleared Chunk, if it points to something else besides deallocChunk_,
                // then FixedAllocator currently has 2 empty Chunks.
                if (NULL != m_emptyChunk)
                {
                    // If last Chunk is empty, just change what deallocChunk_
                    // points to, and release the last.  Otherwise, swap an empty
                    // Chunk with the last, and then release it.
                    Chunk *lastChunk = &m_chunks.back();
                    if (lastChunk == m_deallocChunk)
                        m_deallocChunk = m_emptyChunk;
                    else
                    	if (lastChunk != m_emptyChunk)
                    		std::swap(*m_emptyChunk, *lastChunk);

                    assert(lastChunk->hasAvailable(m_numBlocks));
                    lastChunk->release();
                    m_chunks.pop_back();
                    if ((m_allocChunk == lastChunk) || m_allocChunk->isFilled())
                        m_allocChunk = m_deallocChunk;
                }

                m_emptyChunk = m_deallocChunk;
            }

            // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
            assert((NULL == m_emptyChunk) || (m_emptyChunk->hasAvailable(m_numBlocks)));
        }

        /** Creates an empty Chunk and adds it to the end of the ChunkList.
         All calls to the lower-level memory allocation functions occur inside
         this function, and so the only try-catch block is inside here.
         @return true for success, false for failure.
         */
        bool makeNewChunk()
        {
            bool allocated = false;
//            try
//            {
                std::size_t size = m_chunks.size();
                // Calling chunks_.reserve *before* creating and initializing the new
                // Chunk means that nothing is leaked by this function in case an
                // exception is thrown from reserve.
                if (m_chunks.capacity() == size)
                {
                    if (0 == size)
                    	size = 4;

                    m_chunks.reserve(size * 2);
                }

                Chunk newChunk;
                allocated = newChunk.init(m_blockSize, m_numBlocks);
                if (allocated)
                    m_chunks.push_back(newChunk);
//            }
//            catch ( ... )
//            {
//                allocated = false;
//            }

            if (!allocated)
            	return false;

            m_allocChunk = &m_chunks.back();
            m_deallocChunk = &m_chunks.front();

            return true;
        }

        /** Finds the Chunk which owns the block at address p.  It starts at
         deallocChunk_ and searches in both forwards and backwards directions
         from there until it finds the Chunk which owns p.  This algorithm
         should find the Chunk quickly if it is deallocChunk_ or is close to it
         in the Chunks container.  This goes both forwards and backwards since
         that works well for both same-order and opposite-order deallocations.
         (Same-order = objects are deallocated in the same order in which they
         were allocated.  Opposite order = objects are deallocated in a last to
         first order.  Complexity is O(C) where C is count of all Chunks.  This
         never throws.
         @return Pointer to Chunk that owns p, or NULL if no owner found.
         */
        Chunk *vicinityFind(void *p) const
        {
            if (m_chunks.empty())
            	return NULL;

            assert(m_deallocChunk);

            const std::size_t chunkLength = m_numBlocks * m_blockSize;
            Chunk * lo = m_deallocChunk;
            Chunk * hi = m_deallocChunk + 1;
            const Chunk * loBound = &m_chunks.front();
            const Chunk * hiBound = &m_chunks.back() + 1;

            // Special case: deallocChunk_ is the last in the array
            if (hi == hiBound)
            	hi = NULL;

            for (;;)
            {
                if (lo)
                {
                    if (lo->hasBlock(p, chunkLength))
                    	return lo;

                    if (lo == loBound)
                    {
                        lo = NULL;

                        if (NULL == hi)
                        	break;
                    }
                    else
                    	--lo;
                }

                if (hi)
                {
                    if (hi->hasBlock(p, chunkLength))
                    	return hi;

                    if (++hi == hiBound)
                    {
                        hi = NULL;

                        if (NULL == lo)
                        	break;
                    }
                }
            }

            return NULL;
        }

    private:
        /// Type of container used to hold Chunks.
        typedef std::vector<Chunk, stlsoft::malloc_allocator<Chunk> > Chunks;
        /// Iterator through container of Chunks.
        typedef Chunks::iterator ChunkIter;
        /// Iterator through const container of Chunks.
        typedef Chunks::const_iterator ChunkCIter;

        enum
        {
            /// Fewest # of objects managed by a Chunk.
            MinObjectsPerChunk = 8,
            /// Most # of objects managed by a Chunk - never exceeds UCHAR_MAX.
            MaxObjectsPerChunk = USHRT_MAX
        };

    private:
        /// Number of bytes in a single block within a Chunk.
        std::size_t m_blockSize;
        /// Number of blocks managed by each Chunk.
        size_type m_numBlocks;

        /// Container of Chunks.
        Chunks m_chunks;
        /// Pointer to Chunk used for last or next allocation.
        Chunk *m_allocChunk;
        /// Pointer to Chunk used for last or next deallocation.
        Chunk *m_deallocChunk;
        /// Pointer to the only empty Chunk if there is one, else NULL.
        Chunk *m_emptyChunk;
    };

// GetOffset ------------------------------------------------------------------
/// @ingroup SmallObjectGroupInternal
/// Calculates index into array where a FixedAllocator of numBytes is located.
inline std::size_t GetOffset(std::size_t numBytes, std::size_t alignment)
{
    return (numBytes + alignment - 1) / alignment;
}

// DefaultAllocator -----------------------------------------------------------
/** @ingroup SmallObjectGroupInternal
 Calls the default allocator when SmallObjAllocator decides not to handle a
 request.  SmallObjAllocator calls this if the number of bytes is bigger than
 the size which can be handled by any FixedAllocator.
 @param numBytes number of bytes
 @param doThrow True if this function should throw an exception, or false if it
  should indicate failure by returning a NULL pointer.
*/
inline void *DefaultAllocator(std::size_t numBytes)
{
    return malloc(numBytes);
}

// DefaultDeallocator ---------------------------------------------------------
/** @ingroup SmallObjectGroupInternal
 Calls default deallocator when SmallObjAllocator decides not to handle a
 request.  The default deallocator could be the global delete operator or the
 free function.  The free function is the preferred default deallocator since
 it matches malloc which is the preferred default allocator.  SmallObjAllocator
 will call this if an address was not found among any of its own blocks.
 */
inline void DefaultDeallocator(void *p)
{
    free(p);
}

// SmallObjAllocator::SmallObjAllocator ---------------------------------------

SmallObjAllocator::SmallObjAllocator(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize) :
    m_pool(NULL),
    m_maxSmallObjectSize(maxObjectSize),
    m_objectAlignSize(objectAlignSize)
{
    assert(0 != objectAlignSize);

	stlsoft::malloc_allocator<FixedAllocator> allocator;
    const std::size_t allocCount = GetOffset(maxObjectSize, objectAlignSize);

    if ((m_pool = allocator.allocate(allocCount)) != NULL)
		for (std::size_t i = 0; i < allocCount; ++i)
		{
			allocator.construct(m_pool + i);
			m_pool[i].initialize((i + 1) * objectAlignSize, pageSize);
		}
}

// SmallObjAllocator::~SmallObjAllocator --------------------------------------

SmallObjAllocator::~SmallObjAllocator()
{
	if (m_pool)
	{
		stlsoft::malloc_allocator<FixedAllocator> allocator;

		for (std::size_t i = 0, allocCount = GetOffset(maxObjectSize(), alignment()); i < allocCount; ++i)
			allocator.destroy(m_pool + i);

		allocator.deallocate(m_pool);
	}
}

// SmallObjAllocator::TrimExcessMemory ----------------------------------------

bool SmallObjAllocator::trimExcessMemory()
{
    bool found = false;
    const std::size_t allocCount = GetOffset(maxObjectSize(), alignment());

    std::size_t i = 0;

    for (; i < allocCount; ++i)
        if (m_pool[i].trimEmptyChunk())
            found = true;

    for (i = 0; i < allocCount; ++i)
        if (m_pool[i].trimChunkList())
            found = true;

    return found;
}

// SmallObjAllocator::Allocate ------------------------------------------------

void *SmallObjAllocator::allocate(std::size_t numBytes)
{
    if (numBytes > maxObjectSize())
        return DefaultAllocator(numBytes);

    assert(NULL != m_pool);

    if (0 == numBytes)
    	numBytes = 1;

    assert(GetOffset(numBytes, alignment()) - 1 < GetOffset(maxObjectSize(), alignment()));

    FixedAllocator &allocator = m_pool[GetOffset(numBytes, alignment()) - 1];
    assert(allocator.blockSize() >= numBytes);
    assert(allocator.blockSize() < numBytes + alignment());

    void *place = allocator.allocate();
    if ((NULL == place) && trimExcessMemory())
        place = allocator.allocate();

    return place;
}

// SmallObjAllocator::Deallocate ----------------------------------------------

void SmallObjAllocator::deallocate(void *p, std::size_t numBytes)
{
    if (NULL == p)
    	return;

    if (numBytes > maxObjectSize())
    {
        DefaultDeallocator(p);
        return;
    }

    assert(NULL != m_pool);

    if (0 == numBytes )
    	numBytes = 1;

    assert(GetOffset(numBytes, alignment()) - 1 < GetOffset(maxObjectSize(), alignment()));

    FixedAllocator &allocator = m_pool[GetOffset(numBytes, alignment()) - 1];

    assert(allocator.blockSize() >= numBytes);
    assert(allocator.blockSize() < numBytes + alignment());

    allocator.deallocate(p, NULL);
}

// SmallObjAllocator::Deallocate ----------------------------------------------

void SmallObjAllocator::deallocate(void *p)
{
    if (NULL == p)
    	return;

    assert(NULL != m_pool);

    Chunk *chunk = NULL;
    FixedAllocator *allocator = NULL;

    for (std::size_t i = 0, allocCount = GetOffset(maxObjectSize(), alignment()); i < allocCount; ++i)
        if (NULL != (chunk = m_pool[i].hasBlock(p)))
        {
            allocator = &m_pool[i];
            break;
        }

    if (NULL == allocator)
    {
        DefaultDeallocator(p);
        return;
    }

    assert(NULL != chunk);
    allocator->deallocate(p, chunk);
}

// SmallObjAllocator::IsCorrupt -----------------------------------------------

bool SmallObjAllocator::isCorrupt() const
{
    if ( NULL == m_pool )
    {
        assert( false );
        return true;
    }
    if ( 0 == alignment() )
    {
        assert( false );
        return true;
    }
    if ( 0 == maxObjectSize() )
    {
        assert( false );
        return true;
    }
    const std::size_t allocCount = GetOffset( maxObjectSize(), alignment() );
    for ( std::size_t ii = 0; ii < allocCount; ++ii )
    {
        if ( m_pool[ ii ].isCorrupt() )
            return true;
    }
    return false;
}

} // end namespace Loki

