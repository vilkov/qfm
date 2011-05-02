#ifndef SERVICES_H_
#define SERVICES_H_

#include <QtCore/QString>


class Services
{
public:
	class View
	{
	public:
		virtual ~View() {}

		virtual void view(const QString &filePath) = 0;
	};

	class Edit
	{
	public:
		virtual ~Edit() {}

		virtual void edit(const QString &filePath) = 0;
	};

	class Open
	{
	public:
		virtual ~Open() {}

		virtual void open(const QString &filePath) = 0;
	};


public:
	Services();

	void view(const QString &filePath);
	void edit(const QString &filePath);
	void open(const QString &filePath);

public:
	static Services *instance();
	static void registerService(View *service);
	static void registerService(Edit *service);
	static void registerService(Open *service);
};

#endif /* SERVICES_H_ */
