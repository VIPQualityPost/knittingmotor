---
title: "Component assembly"
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


## 4) Yarn sensor

|Yarn sensor assembly|
| :--: |
|{{< figure src="/knittingmotor/images/CAD_yarn_sensor.png" link="/knittingmotor/images/CAD_yarn_sensor.png" target="blank" title="(click to zoom)" height="400px" width="400px" >}}|

The yarn sensor is the most complex and intricate build of the whole system. As it would be a bit hard to describe how everything goes together,  I suggest taking a closer look at the photos to see how it is done.

### How does it work?

The yarn sensor is capable of sensing a) yarn runout and b) knots in the yarn for the main (right) and the secondary (left)  thread. To achieve this, there are four reed (aka magnetic) contacts on the underside of the sensor which act in combination with the four levers, which are equipped with a small magnet each. Every lever / reed contact combination will theirfor form a single switch and the two sensors (front and back) on each of the sides will form a unit, which means both sides are measured independently, but always the front and the back sensor element together.
So, if either the right runout sensor lever (front) or the right knot sensor lever (back) is engaged, the control box will report an error regarding the right (main) yarn sensor - same for the left (secondary) side.

**Yarn runout**
The switch in the front will be closed when yarn is fed through the sensor, as the thread will hold the corresponding lever up, which will close the reed contact. If the yarn runs out, the lever will drop, which will open the contact and can then be sensed by the control box.<br>
*-> The contacts in the front are "**normally open**" reed contacts.*

**Knot sensing**
If a not comes up from the cone it will be trapped in the small slit in the back lever, raise the lever and, in this case, open the reed contact. This will also lead to a message on the display of the control box.<br>
*-> The contacts in the front are "**normally closed**" reed contacts.*

|Yarn sensor top |Yarn sensor bottom |
| :--: | :--: |
|{{< figure src="/knittingmotor/images/CAD_yarn_sensor_assembly.png" link="/knittingmotor/images/CAD_yarn_sensor_assembly.png" target="blank" title="(click to zoom)" height="300px" width="300px" >}}|{{< figure src="/knittingmotor/images/CAD_yarn_sensor_assembly_underside.png" link="/knittingmotor/images/CAD_yarn_sensor_assembly_underside.png" target="blank" title="(click to zoom)" height="300px" width="300px" >}}|

I recommend taking a look at the [pictures](/knittingmotor/rl/) I took during the build so that you can see how the reed contacts have been placed and how the wiring is done.

### Assembly

- The main plate consists of two parts, `yarn sensor base.stl` and `yarn sensor base screw mount.stl` to make it easier to print. You have to insert the `yarn sensor base screw mount.stl`from the top of the base plate (mind its orientation, the fin has to point to the rear of the plate!) and glue it in place. Make sure it is sitting flush and straight in its position.
- Glue the reed contacts into place (small recesses). That means, the two "normally open" contacts in the front and the two "normally closed" ones two the back. Double check that you are about to glue the right ones into place!
- Then, on either side, connect the front contact two the back contact. If you take a look at the [photo](/knittingmotor/rl/pictures/yarn_sensor_build_1.jpg) I took during the build you can see, that I put a little piece of adhesive aluminium tape under the connection and soldered both contacts as flat as possible together. If you want to do it the same way, keep in mind, that you solder above plastic.
- Next, begin to attach the cables to the contacts. See [this photo](/knittingmotor/rl/pictures/yarn_sensor_build_2.jpg) how I did that. Guide the front cables to the back, then attach the back cables. You should secure the cables by glueing them right to main sensor plate, so that they will not interfere with the levers. There is not much room for everything, but I think you get the point. I used a little bit of shrink tube to make sure nothing comes in contact with each other.
- After that, it is just a matter of a bit of smart cable management - I used a little bit of a somewhat stiffer shrink tube to [*model* the way the cables go](/knittingmotor/rl/pictures/yarn_sensor_build_3.jpg) between the back lever arms. As soon as the shrink tube cooles down, the cables will be fixed and won't interfere with anything anymore.
- Now, assemble the levers and add the magnets to the little tubes, but don't glue them into place right now, we have to fine tune their position first.

*(If you take a look at the CAD images you will see that assembling the moving parts actually is quite straight forward. I used nylon washers between everything to have as little friction as possible. You can also use thin metal washers, but you cannot omit them completely.)*

#### Adjust the mechanical play
Use a nylon lock nut along with the central screw to adjust the play of the lever movement. The four levers should move up and down freely, but make sure that they don't wiggle around sideways, as this will lead to unreliable behaviour, especally with the back (knot) sensor elements. Using a lock nut is a good way to adjust the play in very small steps, as the difference  between too loose and too tight is rather small.

#### Fine tuning of the magnet positions

The magnets have to be placed inside the small tubes of the front and back levers in a way that moving the levers up and down will engage or disengage the corresponding reed contact directly above them. I can only give a general advice for calibrating the magnet position,  because I don't know the actual strength of the magnets you bought, but you have to position the magnets in such a way, that

- the front levers can actually move a little bit up and down without disengaging the reed contact, but open the contact when you let them swing down completely
- the back levers (which have much less room to move up and down!) should open the reed contact *only* when the lever is really close or touching the reed contact

If the force of the magnets is too strong, it can help to move them sideways in relation to the reed contact inside the tube to reduce the magnetic field impact. Use a multimeter connected to the back and front reed contact to see if it works. The multimeter should detect a closed connection when a) the front lever is in up and b) the corresponding back lever in its down position.

If you are happy with how everything works, simply glue the magnets in place so that they cannot leave their calibrated position anymore. To even more secure the magnets you can insert one of the small caps on either side of the tube and glue them into place, too.

## 3) Stepper and rotary encoder assembly

|Stepper assembly|Rotary encoder assembly|
| :--: | :--: |
| {{< figure src="/knittingmotor/images/CAD_stepper_assembly.png" link="/knittingmotor/images/CAD_stepper_assembly.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} | {{< figure src="/knittingmotor/images/CAD_rotary_encoder_assembly_detail.png" link="/knittingmotor/images/CAD_rotary_encoder_assembly_detail.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} |

Assembling the attachement for the rotary encoder and completing the whole stepper motor setup is really straight forward. Although the coupler between the rear motor shaft and the rotary encoder will allow for some misalignment, try to make sure that there is no play in the back assembly and that everything is tight and straight.

### Regarding the rotary encoder bracket

The bracket consists of three parts for easier and support-less printing:
- `rotary encoder bracket base left.stl`
- `rotary encoder bracket base right.stl`
- `rotary encoder bracket bridge.stl`

It can be a bit fiddly to get all three parts together and you will have to bend the legs of the bridge a bit until everything snaps together. As a hint, you should first insert the legs of the bridge into the base parts from the top and then lightly force everything into place. I assure you it will work, work slowly and carefully.

## 4) Control box

|Assembly views| |
| :--: | :--: |
| {{< figure src="/knittingmotor/images/CAD_control_box_assembly.png" link="/knittingmotor/images/CAD_control_box_assembly.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} | {{< figure src="/knittingmotor/images/CAD_control_box_upper_parts_connection.png" link="/knittingmotor/images/CAD_control_box_upper_parts_connection.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} |
| {{< figure src="/knittingmotor/images/CAD_control_box_upper_part_inside_view.png" link="/knittingmotor/images/CAD_control_box_upper_part_inside_view.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} | {{< figure src="/knittingmotor/images/CAD_control_box_upper_part_1.png" link="/knittingmotor/images/CAD_control_box_upper_part_1.png" target="blank" title="(click to zoom)" height="100%" width="100%" >}} |

From looking at the CAD images it should be obvious where everything belongs inside the control box. To attach the display mount to the main upper housing, simply slide the two small tabs in the front of the display mount in position and put in the screws to hold the mains connector in place, this will fix everything nice and tidy. It is a bit hard to describe, but take a look at the second image and you should see how this is going together.

The stepper driver is held in place by friction. If your tolerances are a bit too loose you can use a bit of double sided tape to hold it in place.

The power supply is just laying down inside the box. As you would not turn everything upside down or shake the control box like crazy, there is simply no need for screws to hold it in place.

As a sidenote, I know that there is not much room between the mains connector and the underside of the stereo jacks but this is no problem as long as you seriously take care of isolating every single connector. I used relatively thick shrink tubes around everything and it works just fine.

