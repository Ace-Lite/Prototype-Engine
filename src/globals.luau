--!nocheck
-- SETUP

events = {}


-- KEYDOWN

events.keydown = {}
events.keydown.funcs = {}

function events.keydown.add(func)
	events.keydown.funcs:insert(func)
	return #events.gamelogic.funcs
end

function events.keydown.remove(key)
	events.keydown.funcs:remove(key)
end


-- KEYUP

events.keyup = {}
events.keyup.funcs = {}

function events.keyup.add(func)
	events.keydown.funcs:insert(func)
	return #events.gamelogic.funcs
end

function events.keyup.remove(key)
	events.keyup.funcs:remove(key)
end

-- GAMELOGIC

events.gamelogic = {}
events.gamelogic.funcs = {}

function events.gamelogic.add(func)
	events.gamelogic.funcs:insert(func)
	return #events.gamelogic.funcs
end

function events.gamelogic.remove(key)
	events.gamelogic.funcs:remove(key)
end

-- QUIT

events.quit = {}
events.quit.funcs = {}

function events.quit.add(func)
	events.quit.funcs:insert(func)
	return #events.quit.funcs
end

function events.quit.remove(key)
	events.quit.funcs:remove(key)
end

-- EVENTS

local activate = true

function events_manager()

	local keydown = events.keydown.funcs

	if keydownPress then
		local key = keydownPress

		for i = 1, #keydown do
			keydown[i](key)
		end
	end

	local keyup = events.keyup.funcs

	if keyupPress then
		local key = keyupPress

		for i = 1, #keyup do
			keyup[i](key)
		end
	end

	local gamelogic = events.gamelogic.funcs

	for i = 1, #gamelogic do
		gamelogic[i]()
	end


	local quit = events.gamelogic.funcs

	if quitingPress then
		for i = 1, #gamelogic do
			gamelogic[i]()
		end
	end
end