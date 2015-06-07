#ifndef CMOS_H_
#define CMOS_H_

#include <stdint.h>

void cmos_init(void);
void cmos_enable_nmi(void);
void cmos_disable_nmi(void);

uint8_t cmos_get_seconds(void);
uint8_t cmos_get_minutes(void);
uint8_t cmos_get_hours(void);
uint8_t cmos_get_weekday(void);
uint8_t cmos_get_day_of_month(void);
uint8_t cmos_get_month(void);
uint8_t cmos_get_year(void);
uint8_t cmos_get_century(void);

#endif /* CMOS_H_ */