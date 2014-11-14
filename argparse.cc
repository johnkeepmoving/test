/*
 * =====================================================================================
 *
 *       Filename:  argparse.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/2014 02:24:08 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@unitedstack.com
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "argparse.h" 

static void dashes_to_underscores(const char *input, char *output)
{
  char c = 0;
  char *o = output;
  const char *i = input;
  // first two characters are copied as-is
  *o = *i++;
  if (*o++ == '\0')
    return;
  *o = *i++;
  if (*o++ == '\0')
    return;
  for (; ((c = *i)); ++i) {
    if (c == '=') {
      strcpy(o, i);
      return;
    }
    if (c == '-')
      *o++ = '_';
    else
      *o++ = c;
  }
  *o++ = '\0';
}

bool ceph_argparse_flag(vector<const char*> &args,
	vector<const char*>::iterator &i, ...)
{
  const char *first = *i;
  char tmp[strlen(first)+1];
  dashes_to_underscores(first, tmp);
  first = tmp;
  va_list ap;

  va_start(ap, i);
  while (1) {
    const char *a = va_arg(ap, char*);
    if (a == NULL) {
      va_end(ap);
      return false;
    }
    char a2[strlen(a)+1];
    dashes_to_underscores(a, a2);
    if (strcmp(a2, first) == 0) {
      i = args.erase(i);
      va_end(ap);
      return true;
    }
  }
}

static bool va_ceph_argparse_witharg(vector<const char*> &args,
	vector<const char*>::iterator &i, std::string *ret, va_list ap)
{
  const char *first = *i;
  char tmp[strlen(first)+1];
  dashes_to_underscores(first, tmp);
  first = tmp;

  // does this argument match any of the possibilities?
  while (1) {
    const char *a = va_arg(ap, char*);
    if (a == NULL)
      return false;
    int strlen_a = strlen(a);
    char a2[strlen_a+1];
    dashes_to_underscores(a, a2);
    if (strncmp(a2, first, strlen(a2)) == 0) {
      if (first[strlen_a] == '=') {
	*ret = first + strlen_a + 1;
	i = args.erase(i);
	return true;
      }
      else if (first[strlen_a] == '\0') {
	// find second part (or not)
	if (i+1 == args.end()) {
	  cerr << "Option " << *i << " requires an argument." << endl;
	  exit(1);
	}
	i = args.erase(i);
	*ret = *i;
	i = args.erase(i);
	return true;
      }
    }
  }
}

bool ceph_argparse_witharg(vector<const char*> &args,
	vector<const char*>::iterator &i, std::string *ret, ...)
{
  bool r;
  va_list ap;
  va_start(ap, ret);
  r = va_ceph_argparse_witharg(args, i, ret, ap);
  va_end(ap);
  return r;
}
