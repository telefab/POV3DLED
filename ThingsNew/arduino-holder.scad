// Arduino support

// Resolution
$fa = 3; // Minimum angle (lower = higher max resolution, def 12)
$fs = 0.5; // Max fragment size (lower = higher min resolution, def 2)

// Measured parameters
holder_diam = 5.3;
hole_diam = 3.3;
dist_small = 27.94;
dist_large = 48.26;
screw_diam = 3.1;
center_offset = 5.08;

// Support parameters
depth = 2;
hole_space = 3;
screw_space = 2;
hole_size = (holder_diam + depth*2) / 3;

// Generic support
module support(dist, offset) {
	difference() {
		union() {
			// Base
			cube([hole_diam + hole_space * 2, dist + hole_diam + hole_space * 2, depth]);
			// Holder holder
			translate([0, dist/2 + hole_diam/2 + hole_space + offset, holder_diam/2 + depth])
				rotate([0, 90, 0])
					cylinder(r = holder_diam/2 + depth, h = hole_diam + hole_space * 2);
			// Holder screw support
			translate([0, dist/2 + hole_diam/2 + hole_space + offset - (hole_size + depth*2)/2, holder_diam/2 + depth])
				cube([hole_diam + hole_space * 2, hole_size + depth * 2, holder_diam/2 + depth + screw_diam + screw_space*2]);
		}
		// Holder space
		translate([0, dist/2 + hole_diam/2 + hole_space + offset, holder_diam/2 + depth])
			rotate([0, 90, 0])
				cylinder(r = holder_diam/2, h = hole_diam + hole_space * 2);
		// Holder screw support hole
		translate([0, dist/2 + hole_diam/2 + hole_space + offset - (hole_size + depth*2)/2 + depth, holder_diam/2 + depth])
			cube([hole_diam + hole_space * 2, hole_size, holder_diam/2 + depth + screw_diam + screw_space*2]);	
		// Holder screw space
		translate([hole_diam/2 + hole_space, dist/2 + hole_diam/2 + hole_space + offset - (hole_size + depth*2)/2, 2* depth + holder_diam + screw_space + screw_diam / 2])
			rotate([-90, 0, 0])
				cylinder(r = screw_diam/2, h = hole_size + depth * 2);
		// Arduino connection holes 
		translate([hole_space + hole_diam/2, hole_space + hole_diam/2, 0])
			cylinder(r = hole_diam/2, h = depth);
		translate([hole_space + hole_diam/2, hole_space + hole_diam/2 + dist, 0])
			cylinder(r = hole_diam/2, h = depth);
	}
}

// Small support
rotate([0, -90, 0]) 
	support(dist_small, 0);
// Big support
translate([-holder_diam - screw_diam - screw_space * 2 - depth*3, 0, 0])
	rotate([0, -90, 0]) 
		support(dist_large, -center_offset);