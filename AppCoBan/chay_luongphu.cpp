//chay_luongphu.cpp
#include "chay_luongphu.h"
#include "chucnang_cotloi.h"
#include "csdl.h"
#include "logic_giaodien.h"
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

void lp_nap_cauhinh()
{
	pool.enqueue(nap_cauhinh);
}

void lp_xuat_cauhinh(std::string& dd)
{
	pool.enqueue(xuat_cauhinh, dd);
}

void lp_nap_cauhinh_macdinh()
{
	pool.enqueue(ch_macdinh);
}

void lp_chay_saochep_ini()
{
	pool.enqueue(saochep_ini);
}
