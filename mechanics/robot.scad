// ====================================================================
//  Header
// ====================================================================

use <robot_pioneer_p3dx.scad>;
use <din_rail.scad>;
use <2020_profile.scad>;

// ====================================================================
//  Model
// ====================================================================

// Base
pioneer_p3dx();

// Estrutura de Aluminio
translate([0,-40,0]) {
    translate([20,0,125]) 2020_profile(100);
    translate([10,-10,200]) rotate([90,180,90]) din_rail(170);
    translate([170,0,125]) 2020_profile(100);
}

// Eletronicos
color("blue") translate([110,-70,200]) rotate([90,0,0]) 
import("rpi3.stl");