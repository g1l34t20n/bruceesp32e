![Bruce Main Menu](./media/pictures/bruce_banner.jpg)

# :shark: Bruce

Bruce is meant to be a versatile ESP32 firmware that supports a ton of offensive features focusing on facilitating Red Team operations.
It also supports m5stack products and works great with Cardputer, Sticks, M5Cores, T-Decks and T-Embeds.

## :building_construction: How to install
to install on an an elegoo 2.7" cyd with a single usbc port with boot and rst on each side of the c port.  the board says 2.8 LCE Display \n ESP32-32E 240X320 

do a git clone on this repo:
"git repo https://github.com/g1l34t20n/bruceesp32e"

# Pull the latest(redundant but whatever)
git pull origin main

# Clean build
pio run -t clean

# Rebuild
pio run -e ESP32-32E

# Flash to device
pio run -e ESP32-32E -t upload

make sure your uptodate with your python3 and also 
projectIO for the build system


Bruce is a tool for cyber offensive and red team operations, distributed under the terms of the Affero General Public License (AGPL). It is intended for legal and authorized security testing purposes only. Use of this software for any malicious or unauthorized activities is strictly prohibited. By downloading, installing, or using Bruce, you agree to comply with all applicable laws and regulations. This software is provided free of charge, and we do not accept payments for copies or modifications. The developers of Bruce assume no liability for any misuse of the software. Use at your own risk.

