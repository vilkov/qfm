/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "vfs_uri.h"
#include "../../tools/templates/templates_statemachine.h"

//#include <QtCore/QDebug>
#include <stdlib.h>


VFS_NS_BEGIN

class Parser
{
public:
	enum State
	{
		Shema,
		Colon,
		Slash,
		UserName,
		Password,
		Domain,
		Port,
		Path,
		Stoped
	};

public:
	Parser(const QByteArray &path) :
		m_valid(false)
	{
		/**
		 * XXX: Colon (:) is a valid character in UNIX file name
		 * but we ignore it here.
		 */
		typedef RULE(Shema,    '/', Path,      Parser, path)      Rule1;
		typedef RULE(Shema,      0, Stoped,    Parser, path)      Rule2;
		typedef RULE(Shema,    ':', Colon,     Parser, shema)     Rule3;
		typedef RULE(Colon,    '/', Slash,     Parser, doNothing) Rule4;
		typedef RULE(Slash,    '/', UserName,  Parser, doNothing) Rule5;

		typedef RULE(UserName, '/', Path,      Parser, domain)    Rule6;
		typedef RULE(UserName,   0, Stoped,    Parser, domain)    Rule7;

		typedef RULE(UserName, ':', Password,  Parser, userName)  Rule8;
		typedef RULE(UserName, '@', Domain,    Parser, userName)  Rule9;
		typedef RULE(Password, '@', Domain,    Parser, password)  Rule10;
		typedef RULE(Password, '/', Path,      Parser, userNameIsDomain_thisIsPort) Rule11;
		typedef RULE(Password,   0, Stoped,    Parser, userNameIsDomain_thisIsPort) Rule12;

		typedef RULE(Domain,   ':', Port,      Parser, domain)    Rule13;
		typedef RULE(Domain,   '/', Path,      Parser, domain)    Rule14;
		typedef RULE(Domain,     0, Stoped,    Parser, domain)    Rule15;

		typedef RULE(Port,     '/', Path,      Parser, port)      Rule16;
		typedef RULE(Port,       0, Stoped,    Parser, port)      Rule17;

		typedef RULE(Path,     '/', Path,      Parser, path)      Rule18;
		typedef RULE(Path,       0, Stoped,    Parser, path)      Rule19;

		typedef LOKI_TYPELIST_19(Rule1,  Rule2,  Rule3,  Rule4,  Rule5,
	  							 Rule6,  Rule7,  Rule8,  Rule9,  Rule10,
	  							 Rule11, Rule12, Rule13, Rule14, Rule15,
	  							 Rule16, Rule17, Rule18, Rule19) Rules;

		m_valid = ::Tools::Templates::StateMachine<Rules>(this).process(path) == Stoped;
	}

private:
	void doNothing(const char *string, int size)
	{}

	void shema(const char *string, int size)
	{
		m_shema = QString::fromUtf8(string, size);
//		qDebug() << "m_shema = " << m_shema;
	}

	void userName(const char *string, int size)
	{
		m_userName = QString::fromUtf8(string, size);
//		qDebug() << "m_userName = "  << m_userName;
	}

	void password(const char *string, int size)
	{
		m_password = QString::fromUtf8(string, size);
//		qDebug() << "m_password = "  << m_password;
	}

	void domain(const char *string, int size)
	{
		m_domain = QString::fromUtf8(string, size);
//		qDebug() << "m_domain = "  << m_domain;
	}

	void port(const char *string, int size)
	{
		m_port = strtol(string, NULL, 10);
//		qDebug() << "m_port = " << m_port;
	}

	void path(const char *string, int size)
	{
		if (size > 0)
			m_path.push_back(QString::fromUtf8(string, size));
#ifndef Q_OS_WIN32
		else
			if (m_path.isEmpty())
				m_path.push_back(QChar(L'/'));
#endif
//		qDebug() << "m_path = "  << QString::fromUtf8(string, size);
	}

	void userNameIsDomain_thisIsPort(const char *string, int size)
	{
		m_domain = m_userName;
		m_userName.clear();
//		qDebug() << "m_domain = " << m_domain << "m_userName = ";
		port(string, size);
	}

private:
	friend class Uri;
	bool m_valid;
	QString m_shema;
	QString m_userName;
	QString m_password;
	ushort m_port;
	QString m_domain;
	QStringList m_path;
};

Uri::Uri(const QString &path) :
	m_valid(false)
{
//	Parser parser1("http://UserName:Password@site.com:3128");
//
//	if (parser1.m_valid)
//		qDebug() << "VALID" << "http://UserName:Password@site.com:3128";
//	else
//		qDebug() << "INVALID" << "http://UserName:Password@site.com:3128";
//
//	Parser parser2("UserName:Password@site.com:3128");
//
//	if (parser2.m_valid)
//		qDebug() << "VALID" << "UserName:Password@site.com:3128";
//	else
//		qDebug() << "INVALID" << "UserName:Password@site.com:3128";
//
//	Parser parser3("http://UserName@site.com:3128");
//
//	if (parser3.m_valid)
//		qDebug() << "VALID" << "http://UserName@site.com:3128";
//	else
//		qDebug() << "INVALID" << "http://UserName@site.com:3128";
//
//	Parser parser4("UserName@site.com:3128");
//
//	if (parser4.m_valid)
//		qDebug() << "VALID" << "UserName@site.com:3128";
//	else
//		qDebug() << "INVALID" << "UserName@site.com:3128";
//
//	Parser parser5("http://site.com:3128");
//
//	if (parser5.m_valid)
//		qDebug() << "VALID" << "http://site.com:3128";
//	else
//		qDebug() << "INVALID" << "http://site.com:3128";
//
//	Parser parser6("site.com:3128");
//
//	if (parser6.m_valid)
//		qDebug() << "VALID" << "site.com:3128";
//	else
//		qDebug() << "INVALID" << "site.com:3128";
//
//	Parser parser7("http://site.com");
//
//	if (parser7.m_valid)
//		qDebug() << "VALID" << "http://site.com";
//	else
//		qDebug() << "INVALID" << "http://site.com";
//
//	Parser parser8("site.com");
//
//	if (parser8.m_valid)
//		qDebug() << "VALID" << "site.com";
//	else
//		qDebug() << "INVALID" << "site.com";
//
//	Parser parser9("site.com/home/dav");
//
//	if (parser9.m_valid)
//		qDebug() << "VALID" << "site.com/home/dav";
//	else
//		qDebug() << "INVALID" << "site.com/home/dav";
//
//	Parser parser10("/home/dav");
//
//	if (parser10.m_valid)
//		qDebug() << "VALID" << "/home/dav";
//	else
//		qDebug() << "INVALID" << "/home/dav";

	const Parser parser(path.toUtf8());

	if (parser.m_valid)
	{
		m_valid    = true;
		m_shema    = parser.m_shema;
		m_userName = parser.m_userName;
		m_password = parser.m_password;
		m_port     = parser.m_port;
		m_domain   = parser.m_domain;
		m_path     = parser.m_path;
	}
}

Uri::Uri(const QByteArray &path) :
	m_valid(false)
{
	const Parser parser(path);

	if (parser.m_valid)
	{
		m_valid    = true;
		m_shema    = parser.m_shema;
		m_userName = parser.m_userName;
		m_password = parser.m_password;
		m_port     = parser.m_port;
		m_domain   = parser.m_domain;
		m_path     = parser.m_path;
	}
}

VFS_NS_END
