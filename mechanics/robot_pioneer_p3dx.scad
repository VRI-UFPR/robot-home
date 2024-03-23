
module pioneer_p3dx() {
    // Corpo
    color("red") translate([20,-100,20]) cube([150,200,100]);
    color("black") translate([0,-110,120]) cube([190,220,5]);

    // Rodas 
    module roda() {
        color("black") translate([0,0,50]) rotate([0,90,0]) cylinder(h=20,r=50);
        color("gold") translate([-1,0,50]) rotate([0,90,0]) cylinder(h=22,r=30);
    }

    roda();
    translate([170,0,0]) roda();
}

pioneer_p3dx();