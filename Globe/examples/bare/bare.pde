#include <Globe.h>
#include <Pin.h>

Globe *globe = Globe::get();

void setup() {
	// This is enough to keep the globe constantly refreshing
	globe->setup();
}

void loop() {

}