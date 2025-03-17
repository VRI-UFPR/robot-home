//-------------------------------------------------------------------
// title: DIN clip
// author: OK1HRA 
// license: Creative Commons BY-SA
// URL: http://remoteqth.com/3d-din-rail-mount-clip.php
// revision: 0.1
// format: OpenSCAD
//-------------------------------------------------------------------
// HowTo:
// After open change inputs parameters and press F5
// .STL export press F6 and menu /Design/Export as STL...
//----------------- Input parameter ---------------------------------


distance = 60.0;   // Second mount screw distance
identation = 3;
$fn=50;
width = 7; 
screw = 1.20;      // radius
expand = 1;      // 0-x
long = 45;

din();

module din(){
	difference() {
		union() {
			hull() {
				translate([15+expand,long-2,0]) {cylinder(h=width, r=2, center=false);}
                translate([12+expand,37.5,0]) {cube([5,1,width], center=false);}
                translate([12+expand,long-1,0]) {cube([1,1,width], center=false);}
            }
			hull() {
				translate([16+expand,35.7,0]) {cylinder(h=width, r=1, center=false);}
                translate([14.2+expand,37.5,0]) {cube([2.8,1,width], center=false);}
            }
			hull() {
				translate([18.5+expand,0.5,0]) {cylinder(h=width, r=0.5, center=false);}
				translate([14.5+expand,3.8,0]) {cylinder(h=width, r=0.3, center=false);}
				translate([14.2+expand,0,0]) {cube([1,1,width], center=false);}
			}
			cube([3.5,4,width], center=false);
            cube([15+expand,2,width], center=false);
			hull() {
				translate([12+expand,4,0]) {cylinder(h=width, r=1, center=false);}
                translate([0,3,0]) {cube([1,1,width], center=false);}
                translate([0,long-1,0]) {cube([13+expand,1,width], center=false);}
            }
			hull() {
                translate([0,40,0]) {cube([14,1,width], center=false);}
                translate([2,distance+14,0]) {cylinder(h=width, r=2, center=false);}
                translate([6,distance+14,0]) {cylinder(h=width, r=2, center=false);}
            }
            translate([-identation,10.5-3.5,0]) {cube([identation+1,7,width], center=false);}
            translate([-identation,10.5-3.5+distance,0]) {cube([identation+1,7,width], center=false);}
        }
        translate([3.5,2.5,-1]) {cylinder(h=width+2, r=0.5, center=false);}
        
        // aretation
        translate([9+expand,-1,width/2]) rotate([-90,0,0]) {
            cylinder(h=7, r=screw, center=false);
        }
        translate([6.15+expand,5,-1]) {cube([5.7,3.9,width+2], center=false);}
        
        // 1. Primeiro Furo para montar o parafuso
        translate([-1-identation,10.5,width/2]) rotate([0,90,0]) {
            cylinder(h=4+expand+identation, r=screw, center=false);
        }
		// translate([3,7.65,-1]) {cube([1.9,5.7,width+2], center=false);}
        
        // 2. mount
        translate([-1-identation,10.5+distance,width/2]) rotate([0,90,0]) {
            cylinder(h=4+expand+identation, r=screw, center=false);
        }
        // translate([3,7.65+distance,-1]) {cube([1.9,5.7,width+2], center=false);}
        
        // 3. Vazio no meio da peça para econimizar plastico
        translate([3,12.65,-1]) {cube([7.0,10.7,width+2], center=false);}
        translate([3,26.65,-1]) {cube([7.0,14.7,width+2], center=false);}
        /*translate([3,44.65,-1]) {cube([5.9,10.7,width+2], center=false);}
        translate([3,58.65,-1]) {cube([4.8,11.0,width+2], center=false);}
        translate([3,72.65,-1]) {cube([4.0,11.0,width+2], center=false);}
        translate([3,86.65,-1]) {cube([3.5,11.0,width+2], center=false);}
        translate([3,100.65,-1]) {cube([3.0,17.0,width+2], center=false);}*/

    }
}


