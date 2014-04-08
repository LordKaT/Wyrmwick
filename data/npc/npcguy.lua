blacksmith = { id = "npcguy", dialog = {}, convChoices = {} }

blacksmith.convChoices["main"] = {
	"Hello. I'm the blacksmith.";
--	alt(
--		"Hello. I'm the blacksmith.",
--		"Howdy. Wanna buy some armor?",
--		"Hi. Can I interest you in our fine range of swords and other sharp implements?",
--	),
	{ "Do you have any defensive armor?",
		"Sorry, we only sell offensive armor." };
	{ "Can you tell me more about your products?",
		DialogTree.jump "about_armor" };
	{ "Bye!",
		"Don't get horribly eaten by giant crabs or anything.",
		DialogTree.bye };
}

blacksmith.convChoices["about_armor"] = {
	"Sure. What do you want to know?";
	{ "What is the armor made of?",
		"Really hard stuff." };
	{ "Never mind, I've suddenly lost all interest in that topic.",
		DialogTree.jump "main" };
}

DialogTree.make(blacksmith.convChoices):attach(blacksmith)
blacksmith.convChoices = nil

npc.register(blacksmith)

--[[ function blacksmith:onActivate()
	local c = game.startTalking()
	c.say("Some pointless text.");
	c.wait()
	c.addChoice("A choice")
	c.addChoice("Another choice")
	print(c.offerChoice())
	c.wait()
	c.say("WHAT IS THAT, SANDVICH? KILL THEM ALL!?");
	c.resetChoices()
	c.addChoice("GOOD IDEA");
	c.addChoice("NO BIG SURPRISE.");
	print(c.offerChoice())
	c.stop()
end
]]
