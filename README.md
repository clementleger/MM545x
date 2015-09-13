# Introduction

This library allows to drive MM545xN Chips which are LED display driver with up to 35 outputs.
They are doing all the constant current regulation.

## Usage

Include MM545x.h in your code and declare a MM545x objet with clock pin and data pin as parameter.
Then, simply give a mask of the leds which should be lit.
