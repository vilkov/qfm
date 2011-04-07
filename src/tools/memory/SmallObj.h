#ifndef LOKI_SMALLOBJ_INC_
#define LOKI_SMALLOBJ_INC_

#include <cstddef>

#ifndef LOKI_DEFAULT_CHUNK_SIZE
#define LOKI_DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef LOKI_MAX_SMALL_OBJECT_SIZE
#define LOKI_MAX_SMALL_OBJECT_SIZE 256
#endif

#ifndef LOKI_DEFAULT_OBJECT_ALIGNMENT
#define LOKI_DEFAULT_OBJECT_ALIGNMENT 4
#endif


namespace Loki
{
    class FixedAllocator;

    /** @class SmallObjAllocator
        @ingroup SmallObjectGroupInternal
     Manages pool of fixed-size allocators.
     Designed to be a non-templated base class of AllocatorSingleton so that
     implementation details can be safely hidden in the source code file.
     */
    class SmallObjAllocator
    {
    public:
         /** Allocates a block of memory of requested size.  Complexity is often
         constant-time, but might be O(C) where C is the number of Chunks in a
         FixedAllocator. 

         @par Exception Safety Level
         Provides either strong-exception safety, or no-throw exception-safety
         level depending upon doThrow parameter.  The reason it provides two
         levels of exception safety is because it is used by both the nothrow
         and throwing new operators.  The underlying implementation will never
         throw of its own accord, but this can decide to throw if it does not
         allocate.  The only exception it should emit is std::bad_alloc.

         @par Allocation Failure
         If it does not allocate, it will call TrimExcessMemory and attempt to
         allocate again, before it decides to throw or return NULL.  Many
         allocators loop through several new_handler functions, and terminate
         if they can not allocate, but not this one.  It only makes one attempt
         using its own implementation of the new_handler, and then returns NULL
         or throws so that the program can decide what to do at a higher level.
         (Side note: Even though the C++ Standard allows allocators and
         new_handlers to terminate if they fail, the Loki allocator does not do
         that since that policy is not polite to a host program.)*/

        /** The only available constructor needs certain parameters in order to
         initialize all the FixedAllocator's.
         @param pageSize # of bytes in a page of memory.
         @param maxObjectSize Max # of bytes which this may allocate.
         @param objectAlignSize # of bytes between alignment boundaries.
         */
        SmallObjAllocator(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize);

        /** Destructor releases all blocks, all Chunks, and FixedAllocator's.
         Any outstanding blocks are unavailable, and should not be used after
         this destructor is called.  The destructor is deliberately non-virtual
         because it is protected, not public.
         */
        ~SmallObjAllocator();

        /**
         @param size # of bytes needed for allocation.
         @return NULL if nothing allocated and doThrow is false.  Else the
          pointer to an available block of memory.
         */
        void *allocate(std::size_t size);

        /** Deallocates a block of memory at a given place and of a specific
        size.  Complexity is almost always constant-time, and is O(C) only if
        it has to search for which Chunk deallocates.  This never throws.
         */
        void deallocate(void *p, std::size_t size);

        /** Deallocates a block of memory at a given place but of unknown size
        size.  Complexity is O(F + C) where F is the count of FixedAllocator's
        in the pool, and C is the number of Chunks in all FixedAllocator's.  This
        does not throw exceptions.  This overloaded version of Deallocate is
        called by the nothow delete operator - which is called when the nothrow
        new operator is used, but a constructor throws an exception.
         */
        void deallocate(void *p);

        /// Returns max # of bytes which this can allocate.
        inline std::size_t maxObjectSize() const { return m_maxSmallObjectSize; }

        /// Returns # of bytes between allocation boundaries.
        inline std::size_t alignment() const { return m_objectAlignSize; }

        /** Releases empty Chunks from memory.  Complexity is O(F + C) where F
        is the count of FixedAllocator's in the pool, and C is the number of
        Chunks in all FixedAllocator's.  This will never throw.  This is called
        by AllocatorSingleto::ClearExtraMemory, the new_handler function for
        Loki's allocator, and is called internally when an allocation fails.
        @return True if any memory released, or false if none released.
         */
        bool trimExcessMemory();

        /** Returns true if anything in implementation is corrupt.  Complexity
         is O(F + C + B) where F is the count of FixedAllocator's in the pool,
         C is the number of Chunks in all FixedAllocator's, and B is the number
         of blocks in all Chunks.  If it determines any data is corrupted, this
         will return true in release version, but assert in debug version at
         the line where it detects the corrupted data.  If it does not detect
         any corrupted data, it returns false.
         */
        bool isCorrupt() const;

    private:
        /// Default-constructor is not implemented.
        SmallObjAllocator();
        /// Copy-constructor is not implemented.
        SmallObjAllocator(const SmallObjAllocator &other);
        /// Copy-assignment operator is not implemented.
        SmallObjAllocator &operator=(const SmallObjAllocator &);

    private:
        /// Pointer to array of fixed-size allocators.
        FixedAllocator *m_pool;

        /// Largest object size supported by allocators.
        const std::size_t m_maxSmallObjectSize;

        /// Size of alignment boundaries.
        const std::size_t m_objectAlignSize;
    };
} // namespace Loki

#endif // end file guardian

