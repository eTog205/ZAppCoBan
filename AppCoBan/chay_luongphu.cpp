//chay_luongphu.cpp
#include "chay_luongphu.h"
#include "chucnang_cotloi.h"
#include "csdl.h"
#include "thread_pool.h"


void lp_chay_capnhat()
{
	csdl c;
	pool.enqueue(capnhat_data, c);
}

void lp_chay_lenhwinget(const std::string& id)
{
	pool.enqueue(chaylenh, id);
}

void lp_chay_lenhwindow()
{
	pool.enqueue(chaylenh_suawindow);
}