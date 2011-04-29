#include <QtCore/QGlobalStatic>

#ifdef Q_OS_WIN

#include <QtCore/qt_windows.h>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVector>


extern "C" int main(int argc, char *argv[]);

template<typename Char>
static QVector<Char*> winCmdLine(Char *cmdParam, int length, int &argc)
{
    QVector<Char*> argv(8);
    Char *p = cmdParam;
    Char *p_end = p + length;
    Char *start, *r;
    int quote;

    argc = 0;

    while (*p && p < p_end)
    {
        while (QChar(*p).isSpace())
            ++p;

        if (*p && p < p_end)
        {
            if (*p == Char('\"') || *p == Char('\''))
            {
                quote = *p;
                start = ++p;
            }
            else
            {
                quote = 0;
                start = p;
            }

            r = start;

            while (*p && p < p_end)
            {
                if (quote && *p == quote)
                {
					++p;
					if (QChar(*p).isSpace())
						break;
					quote = 0;
                }

                if (*p == '\\')
                {
                    ++p;

                    if (*p != Char('\"') && *p != Char('\''))
                        --p;
                }
                else
                    if (!quote && (*p == Char('\"') || *p == Char('\'')))
                    {
                        quote = *p++;
                        continue;
                    }
                    else
                    	if (QChar(*p).isSpace() && !quote)
                    		break;

                if (*p)
                    *r++ = *p++;
            }

            if (*p && p < p_end)
                ++p;

            *r = Char('\0');

            if (argc >= (int)argv.size() - 1)
                argv.resize(argv.size() * 2);

            argv[argc++] = start;
        }
    }

    argv[argc] = 0;

    return argv;
}

extern "C" int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR /*cmdParamarg*/, int cmdShow)
{
	QByteArray cmdParam = QString::fromUtf16((ushort*)GetCommandLineW()).toLocal8Bit();

    int argc = 0;
    QVector<char*> argv = winCmdLine<char>(cmdParam.data(), cmdParam.size(), argc);

    return main(argc, argv.data());
}

#endif
