#include <stdio.h>
#include <math.h>
#define BaseDay 6
int g_baseDayOfYear;
int getDay()
{
    if(g_baseDayOfYear == 6)
    {
        g_baseDayOfYear = 0;
        return 6;
    }
    return g_baseDayOfYear++;
}
int printMonth(int mon, int fday, int days)
{
    int i = 1;
    printf("----------------------%d 月-----------------------\n", mon);
    puts("日\t一\t二\t三\t四\t五\t六");
    while(fday--)printf("\t");
    while(days--)
    {
        if(getDay() == 0) printf("\n");
        printf("%d\t", i++);
    }
    puts("");
    return g_baseDayOfYear;
}
int main()
{
    int i = 0, year, offYear, offDay, runYear, baseDay, daysOfMonth[12] = 
                {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
BEGIN:
    scanf("%d", &year);
    system("cls");
    offYear = year - 2000;//偏移年
    runYear = offYear > 0 ? (abs(offYear)/4) + ( year % 4 ? 1 : 0 ) : abs(offYear/4); //期间的闰年数
    offDay = (abs(offYear)-runYear) * 365 + runYear * 366;//偏移天
    g_baseDayOfYear = baseDay = offYear >= 0 ? (offDay % 7 + BaseDay) % 7
       : 6 - abs(BaseDay + offDay + 1) % 7; //year 的 第一天
    //printf("%d  %d\n", dd, g_baseDayOfYear);
    daysOfMonth[2-1] = year % 4 ? 28 : 29;//闰年十月为29天
    for(i = 0; i < 12; i++) baseDay = printMonth(i+1, baseDay, daysOfMonth[i]);
    goto BEGIN;
    return 0;
}
