blacksmith = { id = "npcguy", dialog = {}, convChoices = {} }

function blacksmith:init()
	self.dialog = DialogTree.make(self.convChoices)
	print "made a dialog\n"
end

function blacksmith:onActivate(context)
	local convo = context.startTalking(self)
	self.dialog:reset()
	self.dialog:run(convo)
	convo.stop()
end

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
		{ "Don't get horribly eaten by giant crabs or anything.", DialogTree.bye } };
}

blacksmith.convChoices["about_armor"] = {
	"Sure. What do you want to know?";
	{ "What is the armor made of?",
		"Really hard stuff." };
	{ "Never mind, I've suddenly lost all interest in that topic.",
		DialogTree.jump "main" };
}

npc.register(blacksmith)
