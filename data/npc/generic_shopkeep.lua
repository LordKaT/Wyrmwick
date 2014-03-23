choice("main", {
	alt(
		"Hello. I'm the blacksmith.",
		"Howdy. Wanna buy some armor?",
		"Hi. Can I interest you in our fine range of swords and other sharp implements?",
	),
	{ "Do you have any defensive armor?",
		"Sorry, we only sell offensive armor." };
	{ "Can you tell me more about your products?",
		goto_choice("about_armor") };
	{ "Bye!",
		{ "Don't get horribly eaten by giant crabs or anything.", bye() };
})

choice("about_armor", {
	"Sure. What do you want to know?";
	{ "What is the armor made of?",
		"Really hard stuff." };
	{ "Never mind, I've suddenly lost all interest in that topic.",
		back() };
}
