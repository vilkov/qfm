#include "vfs_uri.h"
#include "../../tools/templates/templates_statemachine.h"


VFS_NS_BEGIN
using namespace ::Tools::Templates;

Uri::Uri(const QString &path)
{
//	typedef RULE(Shema,     '/', Path,      Uri, path)      Rule1;
//	typedef RULE(Shema,       0, Stoped,    Uri, path)      Rule2;
//	typedef RULE(Shema,     ':', Semicolon, Uri, shema)     Rule3;
//	typedef RULE(Semicolon, '/', Slash1,    Uri, doNothing) Rule4;
//	typedef RULE(Slash1,    '/', UserName,  Uri, doNothing) Rule5;
//
//	typedef RULE(UserName,  '/', Path,      Uri, domain)    Rule6;
//	typedef RULE(UserName,    0, Stoped,    Uri, domain)    Rule7;
//
//	typedef RULE(UserName,  ':', Password,  Uri, userName)  Rule8;
//	typedef RULE(UserName,  '@', Domain,    Uri, userName)  Rule9;
//	typedef RULE(Password,  '@', Domain,    Uri, password)  Rule10;
//	typedef RULE(Password,  '/', Path,      Uri, userNameIsDomain_thisIsPort) Rule11;
//
//	typedef RULE(Domain,    ':', Port,      Uri, domain)    Rule12;
//	typedef RULE(Domain,    '/', Path,      Uri, domain)    Rule13;
//	typedef RULE(Domain,      0, Stoped,    Uri, domain)    Rule14;
//
//	typedef RULE(Port,      '/', Path,      Uri, port)      Rule15;
//	typedef RULE(Port,        0, Stoped,    Uri, port)      Rule16;
//
//	typedef RULE(Path,      '/', Path,      Uri, path)      Rule17;
//	typedef RULE(Path,        0, Stoped,    Uri, path)      Rule18;
//
//
//	typedef TYPELlST_4(Rule1, Rule2, Rule3, Rule4) Rules;
//						Rule6, Rule7, Rule8, Rule9, Rule10,
//						Rule11, Rule12, Rule13, Rule14, Rule15,
//						Rule16, Rule17, Rule18) Rules;

//	typedef TYPELIST_5(int, int, int, int, int) Rules;

//	Rules r;
//	Tools::Templates::StateMachine<Rules>(this).process("");



	parseUri(path);
}

void Uri::doNothing(const char *string, int size)
{

}

void Uri::shema(const char *string, int size)
{

}

void Uri::userName(const char *string, int size)
{

}

void Uri::password(const char *string, int size)
{

}

void Uri::domain(const char *string, int size)
{

}

void Uri::port(const char *string, int size)
{

}

void Uri::path(const char *string, int size)
{

}

void Uri::userNameIsDomain_thisIsPort(const char *string, int size)
{

}

VFS_NS_END
