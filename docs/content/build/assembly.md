---
title: "Assembly"
date:  "2024-03-18"
#menu:
#  main:
#      parent: Build your own!
weight: 7
TableOfContents: true
index: false
---

## What you'll find here and what not...

Most of the time I will not give an exact type, length or amount of screws, nuts and bolts. It is up to you to choose the right parts, especially the lengths of the screws.

But as a rule of thumb

- use washers wherever appropriate (don't force the screw heads into the plastic)
- screws for the basic frame are M5
- the carriage mover needs M3-M6
- the belt tensioner M4
- most of the other parts M3. 

Depart from that, I will be more precise in case it is necessary to maintain functionality.

If your T-nuts can only be slid in from the sides of the extrusion, make sure that you have enough T-nuts in place before beginning to screw things down. Don't ask me how I know, that you would have to disassemble more or less parts again, if a T-nut is missing...

Regarding the 3D printed parts, I will use the names of the STL files wherever a 3D printed part has to be placed. That should make it easier to identify their usage.

------

## 1) Frame


|Frame only|
| :--:|
|{{< figure src="/knittingmotor/images/CAD_frame.png" link="/knittingmotor/images/CAD_frame.png" target="blank" title="(click to zoom)" height="40%" width="40%" >}}|

I left out the knitting machine in this picture for better visual clarity. In reality, because of the table clamps, the machine has to be in place already, because you simply cannot screw down the clamps to the table and fit the machine afterwards as you might already know.

Assembling the frame first and bolting everything to the table later is totally possible though. I screwed the whole frame together, fixed  the machine to the table and positioned the frame. Here is how...

<u>The frame itself</u>

- The frame is straightforward. Use the corner brackets to connect the front rest, feet and mount post extrusions and make sure everything is square. I only used the corner brackets, but for added stability you could drill holes in the front rest, cut threads into the feet and screw them together. But it really isn't necessary.
  Regarding the mount posts, you can start about 105mm from the front rest bar as a starting point. You will have to fine adjust their horizontal (back to front) position later, when the carriage mover is attached.
- Add the `T-joining plate.stl`s at the sides for stability and stiffness.
- Loosely screw the `barclamp.stl`s in combination with the `vertical stiffner left.stl` and  `vertical stiffner right.stl` (naming is according to looking at them from beind the machine) to the top run bar, then slide the clamps over the mount posts on both sides, leaving about 5mm of the mount post free above. Lock the barclamps and stiffners down, but make sure, the run bar is positioned equally distant over the knitting machine. The position is only a starting point. You may have to adjust the height of the run bar later when attaching the carriage mover though.

|Post back|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_post_back.png" link="/knittingmotor/images/CAD_post_back.png" target="blank" title="(click to zoom)" height="200px" width="200px">}}|

- Slide the two `belt slider.stl` over the top of the mount posts and see if everything fits together nicely. The two sliders are for general protection and to hide the cable for the max endstop later.

|Post back with slider|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_post_back_with_belt_slider.png" link="/knittingmotor/images/CAD_post_back_with_belt_slider.png" target="blank" title="(click to zoom)" height="200px" width="200px" >}}|

- Add the last two corner brackets to the top groove of the front rest bar and slide them both to the middle of the bar. Their upright edges have to point away from each other.

<u>Mounting everything to the table</u>

- Use the ribber clamps to fix the knitting machine to the table.

- Now positions the frame over the machine, so that the front rest bar sits on the machine clamp (see picture) and that the machine is evenly spaced between the feet of the the frame.
  And before you ask, yes, that will tilt the whole assembly a slight amount to the back and that's fine :-)

- Slide the two corner brackets in the top groove outwards until they hit the ribber clamps and lock them down - they won't have to be removed later, if you want to disassemble the motor frame.

- Slide  `main clamp left.stl` and `main clamp right.stl`into place and screw them down with an M5 bolt, supported via a `M5_screw_knob.stl`. These two have those nice knobs so that it is easy to remove the whole frame later for disassembly.

|Front rest with clamp|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_front_rest_with_with_clamp.png" link="/knittingmotor/images/CAD_front_rest_with_with_clamp.png" target="blank" title="(click to zoom)" height="200px" width="200px" >}}|

## 2) Carriage mover

|Mover front | Mover back |
| :--: | :--: |
|{{< figure src="/knittingmotor/images/CAD_mover_front.png" link="/knittingmotor/images/CAD_mover_front.png" target="blank" title="(click to zoom)" height="300px" width="300px" >}}|{{< figure src="/knittingmotor/images/CAD_mover_back.png" link="/knittingmotor/images/CAD_mover_back.png" target="blank" title="(click to zoom)" height="500px" width="500px" >}}|

See the following pictures on how to assemble the mover.

- The screws I left intentionally in the picture have to be M6.
- The gantry is fixed by using M5 screws.
- For the `belt holders left/right.stl` use M3 screws and support them with washers.
- The `overload sensor mount.stl` does need an addtional M3 screw, but this one can be screwed in to the plastic.

If you bought a 4-wheel 2020 gantry, simply reassemble it to a 3-wheel version.

***BEWARE: I MISSED THE `endstop bracket.stl` and the `belt holders left/right.stl` have become a new version instead of the shown ones in the exploded-views, but I think you get the point from the front view picture above.***

|Mover assembly front|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_mover_assembly_front.png" link="/knittingmotor/images/CAD_mover_assembly_front.png" target="blank" title="(click to zoom)" height="400px" width="400px" >}}|

- You don't have to attach the `overload sensor mount.stl` and the little switch right now. You can add it later while assembling the electronics. The switch cable though has to be guided on the left side around the `mover plate.stl`to the front and can then be positioned under the `endstop bracket.stl` and guided to the top of the plate. If this makes no sense to you atm, wait until we assemble the electronics.

|Mover assembly back|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_mover_assembly_back.png" link="/knittingmotor/images/CAD_mover_assembly_back.png" target="blank" title="(click to zoom)" height="400px" width="400px" >}}|

- The small `spring presser.stl`, which helps compressing the spring with the tensioner screw, is shown only once on the pictures. If you find that your compression spring is not strong enough, before grabbing a new one, add a second `spring presser.stl` on the top of the spring. This might help already.

If everything went together well, try to slide it over the run bar to see if it runs smoothely and without any wobble or wiggle. If it is too tight or too loose, you have to adjust the position of the rolers. If you don't know how, [OpenBuilds setup video on YT, beginning 2:38](https://youtu.be/pMtHy7sDNG4?t=159) is your friend, but as a hint, normally, some of the rolers are mounted excentrically and you can adjust their position.

{{< youtube "pMtHy7sDNG4?start=158" >}}

## 3) Stepper and rotary encoder

## 4) Control box

