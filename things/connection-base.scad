// Connection base
// This file contains 2 separate elements:
// * a static base, fixed to the motor;
// * a rotating cylinder, fixed to the sphere.
// It is used to add a LED and light sensor to detect a reference angle.

// Wire
wire_diam = 1.8;
// Motor base
base_diam = 63;
base_height = 4.0;
// Base screw
base_screw_diam = 4.5;
base_screw_sep = 48.5;
// Base hole
base_hole_height = base_height;
base_hole_diam = 25.7;
// Rotating cylinder
cyl_offset = base_height + 1;
cyl_height = 15;
cyl_in_diam = 20.2;
cyl_width = 2.5;
cyl_wire_width = 2.5;
cyl_wire_space = 14;
// Rotating screw
cyl_screw_offset = 13.3;
cyl_screw_diam = 4.1;
// light sensor
sensor_offset = 0.5;
sensor_diam = 7;
// LED cache
cache_width = 2.5;
cache_height = cyl_offset + sensor_offset + sensor_diam + 1;
cache_diam = cyl_in_diam + (cyl_width + cyl_wire_width)*2 + 1.5;
// LED box
led_length = 11;
led_diam = 7;
led_box_width = 2;

// Non-moving part
module base() {
	difference() {
		union() {
			difference() {
				union() {
					difference() {
						// Base
						cylinder(h = base_height, r = base_diam/2);
						// Bottom hole
						cylinder(h = base_hole_height, r = base_hole_diam/2);
						
					}
					// LED circular cache
					difference() {
						// Cylinder
						cylinder(h = cache_height, r = cache_diam/2 + cache_width);
						// Inside diameter
						cylinder(h = cyl_height, r = cache_diam/2);
					}
					// LED box
					translate([cache_diam/2, - led_diam/2 - led_box_width, 0]) {
						difference() {
							cube([led_length + led_box_width, led_diam + 2 * led_box_width, cache_height]);
							translate([0, led_box_width*2, led_diam/2 + base_height + led_box_width])
								rotate([0, 90, 0])
									cylinder(h = led_length + led_box_width, r = wire_diam/2);
							translate([0, led_diam, led_diam/2 + base_height + led_box_width])
								rotate([0, 90, 0])
									cylinder(h = led_length + led_box_width, r = wire_diam/2);
						}
					}
				}
				// LED space
				translate([cache_diam/2-1, 0, led_diam/2 + base_height + led_box_width])
					rotate([0, 90, 0])
						cylinder(h = led_length+1, r = led_diam/2);
			}
		}
		// Screws
		translate([0,base_screw_sep/2,0])
			cylinder(h = cache_height, r = base_screw_diam/2);
		translate([0,-base_screw_sep/2,0])
			cylinder(h = cache_height, r = base_screw_diam/2);
	}
}

// Rotating part
module cyl() {
	translate([0, 0, cyl_offset]) {
		union() {
			difference() {
				// Cylinder
				cylinder(h = cyl_height, r = cyl_in_diam/2 + cyl_width + cyl_wire_width);
				// Inside diameter
				cylinder(h = cyl_height, r = cyl_in_diam/2);
				// Wire space
				intersection() {
					cylinder(h = cyl_height, r = cyl_in_diam/2 + cyl_wire_width);
					translate([-(cyl_in_diam/2 + cyl_width + cyl_wire_width), -cyl_wire_space/2, 0])
						cube([cyl_in_diam+(cyl_width+cyl_wire_width)*2, cyl_wire_space, cyl_height]);
				}
				// Screw
				translate([0, 0, cyl_screw_offset + cyl_screw_diam/2 - cyl_offset])
					rotate([90, 0, 0])
						cylinder(h = cyl_in_diam+(cyl_width+cyl_wire_width)*2, r = cyl_screw_diam/2, center=true);
				// Light sensor
				translate([cyl_in_diam/2 + cyl_wire_width - 1, 0, sensor_offset + sensor_diam/2])
					rotate([0, 90, 0])
						cylinder(h = cyl_width + 2, r=sensor_diam/2);
			}
		}
	}
}

// Non-moving part
base();
// Rotating part
//cyl();