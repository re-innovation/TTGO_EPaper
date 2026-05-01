#pragma once
#include "Config.h"

#ifdef QUOTATION_EXAMPLE

#include <stdio.h>
#include <Arduino.h>

String parseQuote(String my_payload);
String parseAuthor(String my_payload);

#endif