#include "cmos.h"
#include "../kernel/low_level.h"
#include "../kernel/system.h"
#include "../kernel/util.h"

#define CMOS_CONTROL		0x70
#define CMOS_DATA			0x71

#define CMOS_STATUS_REG_B	0x0B

int _cmos_nmi_mask = 0;
int _cmos_format_is_bcd = 0;
int _cmos_format_is_24	= 0;

uint8_t _cmos_read_port(int port)
{
	port_byte_out(CMOS_CONTROL, (1 << 7) | port);
}

void _cmos_determine_time_format(void)
{
	uint8_t status = _cmos_read_port(CMOS_STATUS_REG_B);
	
	_cmos_format_is_24 = status & 0x02;
	_cmos_format_is_bcd = status & 0x04;
	
	char *buf = "     ";
	int_to_string(_cmos_format_is_24, buf);
	print("status 24: ");
	print(buf);
	print("\n");
	
	int_to_string(_cmos_format_is_bcd, buf);
	print("status 24: ");
	print(buf);
	print("\n");
}

uint8_t _cmos_convert_bcd_value(uint8_t bcd)
{
	return ((bcd & 0xF0) >> 1) + ((bcd & 0xF0) >> 3) + (bcd &0xF);
}

uint8_t _cmos_read_rtc_port(int port)
{
	uint8_t value = _cmos_read_port(port);
	
	if(_cmos_format_is_bcd) {
		value = _cmos_convert_bcd_value(value);
	}
	
	if(_cmos_format_is_24 && value & 0x80) {
		value = ((value & 0x7F) + 12) % 24;
	}
	
	return value;
}

void cmos_init(void)
{
	//cmos_enable_nmi();
	_cmos_determine_time_format();
}

void cmos_enable_nmi(void)
{
	_cmos_nmi_mask = 1 << 7;
}

void cmos_disable_nmi(void)
{
	_cmos_nmi_mask = 0;
}

uint8_t cmos_get_seconds(void)
{
	uint8_t value = _cmos_read_rtc_port(0x00);
}

uint8_t cmos_get_minutes(void)
{
	return _cmos_read_rtc_port(0x02);
}

uint8_t cmos_get_hours(void)
{
	return _cmos_read_rtc_port(0x04);
}

uint8_t cmos_get_weekday(void)
{
	return _cmos_read_rtc_port(0x06);
}

uint8_t cmos_get_day_of_month(void)
{
	return _cmos_read_rtc_port(0x07);
}

uint8_t cmos_get_month(void)
{
	return _cmos_read_rtc_port(0x08);
}

uint8_t cmos_get_year(void)
{
	return _cmos_read_rtc_port(0x09);
}

uint8_t cmos_get_century(void)
{
	return _cmos_read_rtc_port(0x32);
}