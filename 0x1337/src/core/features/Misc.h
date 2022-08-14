#pragma once

#include "../interfaces.h"
#include <chrono>

namespace Misc {
	typedef std::chrono::system_clock timer;

	inline timer::time_point lastTime;
	inline bool timerStarted = false;

	void bunnyHop(CUserCmd*) noexcept;

	void prepareRevolver(CUserCmd*) noexcept;


	void gayMessages() noexcept;
	
	inline std::vector<std::string> messages = {
			"Welcome to the Cum Zone",

			"Only cum inside anime girls",

			"Quivering clit",

			"Double jointed pussy",

			"Fresh balls",

			"Elegant ejaculation",

			"First the kiss......Then the cum",

			"My dick is in love with pain",

			"Co-op cock torture",

			"Stuff my dick into a furnace",

			"Stitch my cock shut",

			"Pressure cook my greasy balls",

			"Cumblast me, And make it snappy",

			"coom",

			"CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM CUM",

			"I love jizzing on my mans feet",

			"What's all the cummotion?",

			"My dad fell into a cum shaft",

			"My dad glazed my face with cum",

			"Fertilize a baby with hunk spunk",

			"Cum spunk in my trunk",

			"Cum craving toddler",

			"Cum drippin cunt",

			"Cummy Rae Jepsen",

			"Cum me baby",

			"Cummy bottom boy",

			"Night of the Living Cum",

			"Nefarious cum mastermind",

			"Cum makes me fearless",

			"Cum crammer",

			"Cock slammer",

			"Cum slammed ya mum",

			"Mail your mum pieces of my dick",

			"Bazinga!",

			"Chug the cum",

			"Fug ya mum",

			"Fuck my asshole full of cum",

			"Three little words...",

			"Get. Fucked. Nerd.",

			"Cum stuffer",

			"Jenkem huffer",

			"Fuck my cum puddle",

			"Bottom stuffer",

			"Semen huffer",

			"Would love a gator to fuck me",

			"Undercooked baby pig penises",

			"Help my dogs get a huge boner",

			"Waterbong full of cat cum",

			"Accidentally fucked my own ass",

			"I barely had any dicks inside me",

			"Who ate all my cum? A mystery",

			"Cum detective hot on the trail",

			"Bees make honey",

			"I make cummy",
		
			"Im daddy's little whore, and he is mine",

			"It was a mistake to cum here",

			"Your cum sucks dick",

			"A splash of cum to seal the deal",

			"Counsel of cum",

			"All-consuming Lord of Cum",

			"Dick. Heir of the Cum Throne",
			"Swallow your rod",
			"Cum-suck God",
			"Cum-Messiah",
			"Give me the Cum Scar",
			"Tight fuckable dickhole",

			"Cum-cruiser",

			"How wet is your pussy, exactly?",
			"Man, shut the fuck up nerd",
			"Pub-dunked by my white fat cock",
			"Your GF's pussy tastes like my cum",
			"I actually beat a nerd to death",
			"Tired of cumming in nerd pussy",

			"Congratulations, future Cumlords!",
			"I shape fates and I crush dudes",
			"Get paid money to worship Satan",
			"Making this much cum ain’t easy",

			"Execution by excessive cum",
			"You gonna get pounded and creamed",
			"Russian Roulette with a Cum-Gun",
			"You've got one sick dick, bitch",

			"Cum-drooling, moaning slut",
			"Stuff some cum in my butt",
			"Suck my big fat juicy ballsack",
			"You will suck cum from my dick",
			"Ass is running on cum-vapors",
			"Hands off the cum stash, bubs",
			"Speedy cum-dealer",
			"Fucked child",
			"Just chillin'",
			"Cum villain",
			"Cum all ye faithful",
			"Cum-fucked God",
			"In his asshole",
			"Cum proud, cum loud",
			"Fucked up? Now, you’re CUM",
			"Make bank, smoke dank",
			"Stop the cum ocean",

			"Slurp cum",
			"Then, I burp cum",
			"Living large, eat cum, and smile",

			"Eat a cum nuke, baka bitch",

			"Father of lies",
			"Cum in disguise",
			"Your cum won't last",
			"There's a snake in my ass",
			"The cum-fathers secret stash",
			"Cum-stomp me flat",
			"I'm going to fuck your dad",
			"Cumming high into the morning sky",
			"Vape cum from my bum till I die",
			"Watching Arthur with a cock in my ass",
			"Riding hard",
			"Eating ass master-class",
			"Sacred cum blade",
			"A fuckling crusade",
			"Fatal cum--theft",
			"Give me cum or give me death",
			"Elon's Musk",
			"Jesus Crust",
			"Stealing donations from the Cum-Czar's trust",
			"A cum smoothie gulped smoothly",
			"Consume the cum chalice",
			"Fuck everyone named Alex",
			"David Hayter",
			"Cum Crusader",
			"The Holy Cum Wars",
			"Razor-blade Masturbator",
			"Margaret Thatcher",
			"The Cum Snatcher",
			"Father drowned",
			"Going down on the cum clown",
			"Prolapse pounding",
			"Toothpick sounding",
			"Cum baking",
			"My nipple-pussy is aching",
			"Cum fooler",
			"Semen drooler",
			"Forbidden cum-spice",
			"Your shit-box looks nice!",
			"Life is a cage, and death is the key",
			"All your cum are belong to me",
			"Normalize crying over spilt cum",
			"Making cum-angels with my son",
			"I fucked a fairy in half",
			"How many holes does a human have?",
			"My butt and cunny are in agony",
			"Castration in the sky",
			"Your penis will fly",
			"Scrotal chambers",
			"Semen sailors",
			"Mommy's cum tax",
			"Grind my balls on an axe!",
			"Cum-scented candle",
			"Cum-broiled eggs",
			"Cum-Christ consciousness",
			"Third-eye, cum spy",
			"Cum-scrote sailboat",
			"Semen speed racer",
			"Off-road cum chode",
			"My uterus came out!",
			"Cum treasurer",
			"Dick measurer",
			"Irresponsible Manager of Cum",
			"A cum-slave, back from the grave",
			"The price for breaking a cum-oath",
			"James Hector",
			"Cum key-lector (or collector, vote in comments)",
			"I tripped in the cum-keeper's crypt",
			"Cum feeder",
			"Moist meter",
			"Sans Undertale, the cum reaper",
			"Fucking a skeleton, right in the pussy",
			"The Dark Souls of cum",
			"Cum-framed, and cum-blamed",
			"Cum-drowning awareness day",
			"Brewing cum-fuel after school",
			"Your nipples are crunchy",
			"The tragic cum-sponge",
			"Your cum is fading...",
			"Sweep up the cum flakes, Joanv",

			"All warfare is based- Sun Tzu",
			"fuck me daddy",
			"cum in my bumm",
			"anyone here down to fuck me in my supple bussy?",
			"don't apologize to me, apologize to my balls",
			"i'm not mad, i'm just horny",
			"50 shades of gargle my scrotum",
			"i can't stop farting on balls",
			"is it gay if I fuck my cousin?",
			"i'm gonna rim you so hard",
			"This is paul's abortion clinic and pizzeria, where yesterday's loss is todays sauce. How can I help you today?"


	};
}