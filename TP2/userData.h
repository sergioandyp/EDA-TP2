#pragma once

#ifndef USERDATA_H
#define USERDATA_H

typedef struct userDataType {
	int height;
	int width;
	int robots;
	int mode;
	struct userDataType *Flags; //In case of repeated key-value pairs.
}userData;

#endif