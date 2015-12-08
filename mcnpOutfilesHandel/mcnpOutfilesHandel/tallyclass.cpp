#include "tallyclass.h"

PhantomInfo::PhantomInfo()
{
	organvol_count.clear();
	organwet_count.clear();
}

TallyInfo::TallyInfo()
{
	tallytype = 0;
    isStar = false;
	value = 0;
	cellserial = -1;
	relativeerror = 0;
}