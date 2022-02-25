#include <linux/input.h>

#pragma once

#define LABEL(constant) { #constant, constant }
#define LABEL_END { NULL, -1 }

struct label {
	const char* name;
	int value;
};

static struct label ev_labels[] = {
		LABEL(EV_SYN),
		LABEL(EV_KEY),
		LABEL(EV_REL),
		LABEL(EV_ABS),
		LABEL(EV_MSC),
		LABEL(EV_SW),
		LABEL(EV_LED),
		LABEL(EV_SND),
		LABEL(EV_REP),
		LABEL(EV_FF),
		LABEL(EV_PWR),
		LABEL(EV_FF_STATUS),
		LABEL_END,
};