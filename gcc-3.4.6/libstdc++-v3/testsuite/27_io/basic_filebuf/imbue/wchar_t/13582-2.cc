// 2004-01-11  Petur Runolfsson  <peturr02@ru.is>

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 27.8.1.4 Overridden virtual functions

#include <fstream>
#include <locale>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <testsuite_hooks.h>

// libstdc++/13582
void test01()
{
  bool test __attribute__((unused)) = true;
  using namespace std;
  using namespace __gnu_test;

  locale loc_en(__gnu_test::try_named_locale("en_US"));
  locale loc_fr(__gnu_test::try_named_locale("fr_FR"));

  const char* name = "tmp_fifo_13582-2";
  unlink(name);
  try_mkfifo(name, S_IRWXU);
  
  int child = fork();
  if (child == 0)
    {
      filebuf fbout;
      fbout.open(name, ios_base::out);
      fbout.sputn("12345", 5);
      fbout.pubsync();
      sleep(2);
      fbout.close();
      exit(0);
    }

  wfilebuf fbin;
  fbin.open(name, ios_base::in);
  sleep(1);
  wfilebuf::int_type n = fbin.sbumpc();
  VERIFY( n == L'1' );
  fbin.pubimbue(loc_en);
  n = fbin.sbumpc();
  VERIFY( n == L'2' );
  fbin.pubimbue(loc_fr);
  n = fbin.sbumpc();
  VERIFY( n == L'3' );
  n = fbin.sbumpc();
  VERIFY( n == L'4' );
  n = fbin.sbumpc();
  VERIFY( n == L'5' );
  n = fbin.sbumpc();
  VERIFY( n == wfilebuf::traits_type::eof() );
}

int main()
{
  test01();
  return 0;
}
