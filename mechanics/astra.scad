
difference() {
    cube([35,20,9]);
    translate([3.5,8,2.75]) cube([28,7,9]);
    translate([8,10,0]) cube([20,2,2.75]);
    
    translate([9,11,0]) cylinder(9,2);
    translate([27,11,0]) cylinder(9,2);
}

translate([0,20, 0]) 
difference() {
    cube([35,40,4]);
    translate([9,25.6,0]) cube([4,10,9]);
    translate([22,25.6,0]) cube([4,10,9]);
}


/*

translate([5,45,0]) cube([4,10,9]);
translate([26,45,0]) cube([4,10,9]);

translate([4.0,9.9,0]) difference() {
    cube([7,3.2,3]);
    translate([4,1.6,0]) cylinder(3,1);
}

translate([24,9.9,0]) difference() {
    cube([7,3.2,3]);
    translate([3,1.6,0]) cylinder(3,1);
}
*/