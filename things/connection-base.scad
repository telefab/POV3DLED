// Connection base
// This file contains 2 separate elements:
// * a static base, fixed to the motor;
// * a rotating cylinder, fixed to the sphere.
// It is used to add a LED and light sensor to detect a reference angle.

// Resolution
$fa = 3; // Minimum angle (lower = higher max resolution, def 12)
$fs = 0.5; // Max fragment size (lower = higher min resolution, def 2)

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
cyl_in_diam = 20.2;
cyl_width = 2.5;
cyl_wire_width = 2.5;
cyl_wire_space = 10;
// Rotating screw
cyl_screw_offset = 13.3;
cyl_screw_diam = 4.1;
// light sensor
sensor_offset = 0.5;
sensor_diam = 5;
// LED box
led_length = 11;
led_diam = 7;
led_box_width = 2;
// LED cache
cache_width = 2.5;
cache_height = cyl_screw_offset - 0.2;
cache_diam = cyl_in_diam + (cyl_width + cyl_wire_width)*2 + 1;
// Conductor
conductor_diam_in = 31;
conductor_diam_out = 50;
conductor_height = 1;
conductor_sup_height = cyl_screw_diam;
conductor_sup_width = 10;
conductor_space = 3;
conductor_wire_space = 7;
conductor_wire_sep = 1;
conductor_offset = cyl_screw_offset + cyl_screw_diam + 0.2;
conductor_block_width = 3;
// Connector
connector_sup_offset = 1.3;
connector_sup_width = 4;
connector_base_depth = 2.8;
connector_base_width = 9;
connector_base_height = 6;
connector_base_hole_diam = 3.5;
connector_contact_diam = 7;
connector_contact_height = 7.5;
connector_contact_sep = 1;
connector_contact_wall = 0.8;
connector_sup_length = connector_contact_diam + connector_contact_wall*2;
connector_wire_space = 4;
// Cylinder height
cyl_height = conductor_offset + conductor_height * 2 + conductor_space + conductor_block_width + 2 - cyl_offset;

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
							cube([led_length + led_box_width, led_diam + 2 * led_box_width, led_diam + led_box_width + base_height]);
							translate([0, led_box_width*2, led_diam/2 + base_height])
								rotate([0, 90, 0])
									cylinder(h = led_length + led_box_width, r = wire_diam/2);
							translate([0, led_diam, led_diam/2 + base_height])
								rotate([0, 90, 0])
									cylinder(h = led_length + led_box_width, r = wire_diam/2);
						}
					}
				}
				// LED space
				translate([cache_diam/2-1, 0, led_diam/2 + base_height])
					rotate([0, 90, 0])
						cylinder(h = led_length+1, r = led_diam/2);
			}
			// Holding part for the connector
			difference() {
				translate([-conductor_diam_out/2 - connector_sup_width - connector_sup_offset, -connector_sup_length/2 - connector_base_depth, 0])
					cube([connector_base_width, connector_sup_length + connector_base_depth*2, connector_base_height + base_height]);
				translate([-conductor_diam_out/2 - connector_sup_width - connector_sup_offset, -connector_sup_length/2, 0])
					cube([connector_base_width, connector_sup_length, connector_base_height + base_height]);
				translate([-conductor_diam_out/2 - connector_sup_offset - connector_sup_width + connector_base_width/2, connector_sup_length/2 + connector_base_depth, base_height + connector_base_height/2])
					rotate([90, 0, 0])
						cylinder(h = connector_sup_length + connector_base_depth*2, r = connector_base_hole_diam/2);
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
					translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width), -cyl_wire_space/2, 0])
						cube([cyl_in_diam+2*(cyl_width+cyl_wire_width), cyl_wire_space, cyl_height]);
				}
				// Screw
				translate([0, 0, cyl_screw_offset + cyl_screw_diam/2 - cyl_offset])
					rotate([0, 90, 130])
						cylinder(h = cyl_in_diam/2+cyl_width+cyl_wire_width, r = cyl_screw_diam/2);
				// Light sensor
				translate([cyl_in_diam/2 + cyl_wire_width - 1, 0, sensor_offset + sensor_diam/2])
					rotate([0, 90, 0])
						cylinder(h = cyl_width + 2, r=sensor_diam/2);
				// Holes to connect the conductors
				translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width), -conductor_wire_space/2, conductor_offset - cyl_offset + conductor_height])
					cube([cyl_in_diam+2*(cyl_width+cyl_wire_width), conductor_wire_space, conductor_space]);
				// Holes to block the top conductor
				rotate([0,0,50])
					translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width), -conductor_block_width/2, conductor_offset - cyl_offset + 2*conductor_height + conductor_space])
						cube([cyl_in_diam/2+(cyl_width+cyl_wire_width), conductor_block_width, conductor_block_width]);
				rotate([0,0,130])
					translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width), -conductor_block_width/2, conductor_offset - cyl_offset + 2*conductor_height + conductor_space])
						cube([cyl_in_diam/2+(cyl_width+cyl_wire_width), conductor_block_width, conductor_block_width]);
				rotate([0,0,270])
					translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width), -conductor_block_width/2, conductor_offset - cyl_offset + 2*conductor_height + conductor_space])
						cube([cyl_in_diam/2+(cyl_width+cyl_wire_width), conductor_block_width, conductor_block_width]);
				
			}
			// Conductor support
			intersection() {
				translate([0, 0, conductor_offset - cyl_offset - conductor_sup_height]) {
					difference() {
						cylinder(h = conductor_sup_height, r1 = cyl_in_diam/2 + cyl_width + cyl_wire_width, r2 = cache_diam/2 + cache_width);
						cylinder(h = conductor_sup_height, r1 = cyl_in_diam/2 + cyl_wire_width, r2 = cyl_in_diam/2 + cyl_width + cyl_wire_width);
					}
				}
				union() {
					rotate([0, 0, 0])
						translate([-conductor_sup_width/2+0.2, 0, 0])
							cube([conductor_sup_width-0.4, cyl_in_diam + 2*(cyl_width + cyl_wire_width), conductor_offset - cyl_offset]);
					rotate([0, 0, 120])
						translate([-conductor_sup_width/2+0.2, 0, 0])
							cube([conductor_sup_width-0.4, cyl_in_diam + 2*(cyl_width + cyl_wire_width), conductor_offset - cyl_offset]);
					rotate([0, 0, 240])
						translate([-conductor_sup_width/2+0.2, 0, 0])
							cube([conductor_sup_width-0.4, cyl_in_diam + 2*(cyl_width + cyl_wire_width), conductor_offset - cyl_offset]);
				}
			}
		}
	}
}

// Fixed part of the connector
module connector() {
	translate([-conductor_diam_out/2 - connector_sup_width - connector_sup_offset, -connector_sup_length/2, base_height])
	difference() {
		union() {
			// Column
			cube([connector_sup_width, connector_sup_length, conductor_offset+2*conductor_height+conductor_space-base_height+connector_contact_height + connector_contact_wall + connector_contact_sep]);
			// Support
			cube([connector_base_width, connector_sup_length, connector_base_height]);
			// First contact
			translate([connector_sup_width, 0, conductor_offset - base_height - connector_contact_height - connector_contact_wall - connector_contact_sep])
				cube([connector_contact_wall + connector_contact_diam, connector_sup_length, connector_contact_height + connector_contact_wall]);
			// Second contact
			translate([connector_sup_width, 0, conductor_offset - base_height + conductor_space + 2 * conductor_height + connector_contact_sep])
				cube([connector_contact_wall + connector_contact_diam, connector_sup_length, connector_contact_height + connector_contact_wall]);
		}
		// Screw location
		translate([connector_base_width/2, connector_sup_length + connector_base_depth, connector_base_height/2])
			rotate([90, 0, 0])
				cylinder(h = connector_sup_length + connector_base_depth*2, r = connector_base_hole_diam/2);
		// First contact
		translate([connector_sup_width + connector_contact_diam/2, connector_contact_wall + connector_contact_diam/2, conductor_offset - base_height - connector_contact_height - connector_contact_sep])
			cylinder(h = connector_contact_height, r = connector_contact_diam/2);
		// Second contact
		translate([connector_sup_width + connector_contact_diam/2, connector_contact_wall + connector_contact_diam/2, conductor_offset - base_height + conductor_space + 2 * conductor_height + connector_contact_sep])
			cylinder(h = connector_contact_height, r = connector_contact_diam/2);
		// Wire space
		translate([0, (connector_sup_length - connector_wire_space) / 2, conductor_offset - base_height - connector_contact_height - connector_contact_sep])
			cube([connector_sup_width + connector_contact_diam/2, connector_wire_space, 2*conductor_height+conductor_space+2*(connector_contact_height + connector_contact_sep)]);
		
	}
}

// Spacer between the two conductors on the cylinder
module conductor_spacer() {
	translate([0, 0, conductor_offset + conductor_height])
		difference() {
			cylinder(h = conductor_space, r = conductor_diam_out/2);
			cylinder(h = conductor_space, r = cyl_in_diam/2 + cyl_wire_width + cyl_width + 0.5);
			translate([-cyl_in_diam/2-(cyl_width+cyl_wire_width)-(conductor_diam_out-conductor_diam_in)/3, -conductor_wire_space/2, 0])
				cube([cyl_in_diam/2+(cyl_width+cyl_wire_width)+(conductor_diam_out-conductor_diam_in)/3, conductor_wire_space, conductor_space-conductor_wire_sep]);
			translate([0, -conductor_wire_space/2, conductor_wire_sep])
				cube([cyl_in_diam/2+(cyl_width+cyl_wire_width)+(conductor_diam_out-conductor_diam_in)/3, conductor_wire_space, conductor_space-conductor_wire_sep]);
		}
}

// Small bits to block the top conductor
module conductor_block() {
	cube([cache_diam/2 + cache_width - cyl_in_diam/2, conductor_block_width - 0.1, conductor_block_width - 0.1]); 
}

// Conductor simulation
module conductor() {
	difference() {
		cylinder(h = conductor_height, r = conductor_diam_out/2);
		cylinder(h = conductor_height, r = conductor_diam_in/2);	
	}
}

// Non-moving part
color("Green")
	base();
// Rotating part
color("Red")
	cyl();
// Conductor spacer
color("Yellow")
	conductor_spacer();
// Conductor block
color("Yellow")
	translate([30, 30, 0])
		conductor_block();
// Fixed connector
color("Orange")
	connector();
// Conductors simulated
//color("Blue") {
//	translate([0,0,conductor_offset])
//		conductor();
//	translate([0,0,conductor_offset+conductor_height+conductor_space])
//		conductor();
//}