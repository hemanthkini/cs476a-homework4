process = vgroup("Echo",environment{declare name "Echo";

import("stdfaust.lib");

/* ============ DESCRIPTION =============

- Variable de.delay echo
- Echo Delay = Pick manually which amount of time in seconds must be repeated by the echo
- Rocking = To vary the intensity of the echo

*/

process = echo : echoa : echob;


echo = +~ @(echoDelay)*(feedback);

echoDelay = hslider("Echo Delay[unit:s]", 0.5, 0.01, 1, 0.001): min(1): max(0.01)*(44100) : int;
feedback = hslider("Echo Intensity (Feedback)", 0.001, 0.001, 0.65, 0.001):si.smooth(0.999);

echoa = +~ @(echoDelaya)*(feedbacka);

echoDelaya = hslider("Echo Delaya[unit:s]", 0.5, 0.01, 1, 0.001): min(1): max(0.01)*(44100) : int;
feedbacka = hslider("Echo Intensitya (Feedback)", 0.001, 0.001, 0.65, 0.001):si.smooth(0.999);

echob = +~ @(echoDelayb)*(feedbackb);

echoDelayb = hslider("Echo Delayb[unit:s]", 0.5, 0.01, 1, 0.001): min(1): max(0.01)*(44100) : int;
feedbackb = hslider("Echo Intensityb (Feedback)", 0.001, 0.001, 0.65, 0.001):si.smooth(0.999);

}.process);