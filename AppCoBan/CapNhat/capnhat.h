//capnhat.h
#pragma once

#include <string>

using namespace std;

struct capnhat
{
	string phienban_sosanh;
};


string doctep_phienban1(const string& tentep);
void ghitep_phienban(const string& tentep, const string& dl_canghi);

int loc_dl(const string& dauvao);
string dinhdang_dl(int so_phienban);

bool sosanh_phienban(int so_hientai, int so_layve);

void kiemtra_capnhat(const string& phienban);
void kiemtra_va_capnhat();
