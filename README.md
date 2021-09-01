## enhancedCoffeeScale

## Inspiration
I make coffee every morning. Even though I've been doing it for the better part of 2 years, I still struggle to consistently get a good cup due to the number of parameters I need to control. I've simplified things by designing _The Pour Over Assistant_.
## What it does
_The Pour Over Assistant_ helps you with your brew by telling you how much coffee to brew(depending on the amount of beans at the start), what stage of the extraction you are in and how fast to pour. It will also tell you how long the extraction took so you can adjust grind size/water temperature appropriately to achieve your optimal brew time.
## How I built it
I built _The Pour Over Assistant_ with a 5kg load cell, HX711 amplifier, Arduino Uno, 16x02 LCD Display and other basic electronic components. I programmed it on the Arduino coding environment using C++. I designed the casing of the scale with Autodesk Fusion360 and printed it using an Ender-3 printer.
## Challenges I ran into
I originally tried to make the scale track the water going in dynamically instead of cumulatively like a traditional scale. This method proved nearly impossible within the scope of the project so I had to scrap that idea. I fixed this by converting the code to account for cumulative water pouring.
## Accomplishments that I'm proud of
This was my first Arduino project so I'm proud that I have a functioning prototype. I'm really happy that all the hardware and software works great together to produce a tasty cup of coffee!
## What I learned
The Arduino environment is really cool! There are so many things you can do with it and I'm really excited to create more things in the future.
## What's next for _The Coffee Pour Over Assistant_
I definitely need to make the next iteration of this scale out of a heat resistant material like ABS. Since I used PLA for the scale, I had to add an insulating layer of styrofoam which doesn't look great. I will also try to design a PCB for the scale and move away from the breadboard. More options that follow different recipe's is also an option I can program into the scale with minimal alterations.

## Check out this video for the scale in action!
https://www.youtube.com/watch?v=fS8e1eBkPQA
