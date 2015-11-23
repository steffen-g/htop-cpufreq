/*
htop - FreqMeter.c
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.

This meter is written by Steffen L. Graf (S.Graf@gsg-elektronik.de).
*/

#include "FreqMeter.h"

#include "ProcessList.h"
#include "CRT.h"
#include "StringUtils.h"
#include "Platform.h"

#include <string.h>
#include <stdlib.h>

/*{
#include "Meter.h"
#define FREQPATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define MAXPATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
}*/

int FreqMeter_attributes[] = {
   FREQ
};

static void FreqMeter_setValues(Meter * this, char *buffer, int len) {
	unsigned int f_cpu, f_max;
	FILE *fp;

	fp = fopen(FREQPATH, "r");
	fscanf(fp, "%u", &f_cpu);
	this->values[0] = f_cpu;
	fclose(fp);

	fp = fopen(MAXPATH, "r");
        fscanf(fp, "%u", &f_max);
        fclose(fp);

	if(this->mode == TEXT_METERMODE)
		snprintf(buffer, len, "%.3f GHz", this->values[0]/1000000.0);
	else{
		snprintf(buffer, len, "%.1f GHz", this->values[0]/1000000.0);
		this->values[0] /= f_max;
                this->values[0] *= 100;
	}
      return;

}

MeterClass FreqMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete
   },
   .setValues = FreqMeter_setValues,
   .defaultMode = TEXT_METERMODE,
   .total = 100.0,
   .attributes = FreqMeter_attributes,
   .name = "Freq",
   .uiName = "Freq",
   .caption = "Freq: "
};
