let player_id = 0
let pokemon_names = []

function onKeyPressed(event) {
	const enterKeyCode = 13


	if (event.keyCode == enterKeyCode) {
		try_guess_pokemon()
	}
}

function update_image(image_url) {
	document.getElementById("pokemon_image").src = image_url
}

function update_types(types) {
	let types_str = "";
	for (let i = 0; i < types.length; ++i) {
		types_str += "["
		types_str += types[i]
		types_str += "] "
	}
	let element = document.getElementById("pokemon_types")
	element.textContent = element.textContent.replace("???", types_str)
}

function update_weight(weight) {
	let element = document.getElementById("pokemon_weight")
	element.textContent = element.textContent.replace("???", weight.toString())
}

function update_height(height_in_dm) {
	let element = document.getElementById("pokemon_height")
	height = height_in_dm / 10.0;
	element.textContent = element.textContent.replace("???", height.toString())
}

function update_name(name) {
	let element = document.getElementById("pokemon_name")
	element.textContent = element.textContent.replace("???", name)
}

function is_local() {
	return window.location.href.startsWith("file") || window.location.href.indexOf("localhost") != -1;
}

function try_guess_pokemon() {
	let pokemon_name = document.getElementById("title_input").value
	// Capitalize first character
	pokemon_name = pokemon_name.charAt(0).toUpperCase() + pokemon_name.slice(1)
	if (pokemon_name == "") {
		return
	} else if (!pokemon_names.includes(pokemon_name)) {
		alert('This pokemon does not exist.')
	}

	const options = {
		method: 'GET'
	}

	let url;
	if (is_local()) {
		url = new URL("http://localhost:1234/guess")
	} else {
		url = new URL("https://pokeguesser.baduit.eu/guess")
	}
	url.searchParams.append("id", player_id.toString())
	url.searchParams.append("pokemon_name", pokemon_name)

	fetch(url, options)
		.then((response) => {
			return response.text()
		})
		.then((text_response) => {
			console.log(text_response)
			let object_response = JSON.parse(text_response)

			if (object_response.game_status == "finished") {
				document.getElementById("guess_pokemon").disabled = true
			}

			if (object_response.success == true) {
				console.log("You won !")
				update_name(object_response.pokemon.name)
				update_types(object_response.pokemon.types)
				update_image(object_response.pokemon.image_url)
				update_height(object_response.pokemon.height)
				update_weight(object_response.pokemon.weight)
				alert("You won ! :D")
			} else {
				if (object_response.field_name == "types") {
					update_types(object_response.field_value)
				} else if (object_response.field_name == "height") {
					update_height(object_response.field_value)
				} else if (object_response.field_name == "weight") {
					update_weight(object_response.field_value)
				} else if (object_response.field_name == "image_url") {
					update_image(object_response.field_value)
				} else if (object_response.field_name == "name") {
					update_name(object_response.field_value)
					alert("You lost ! :'(")
				} else {
					console.error("Unknown field name")
				}
			}
		})
}

function update_pokemon_list() {
	let element = document.getElementById("pokemon_list")
	for (let i = 0; i < pokemon_names.length; ++i) {
		let new_option = document.createElement("option")
		new_option.value = pokemon_names[i]
		element.appendChild(new_option)
	}
}

function startup() {
	const options = {
		method: 'GET'
	}

	let start_url;
	if (is_local()) {
		start_url = "http://localhost:1234/start"
	} else {
		start_url = "https://pokeguesser.baduit.eu/start"
	}

	fetch(start_url, options)
		.then((response) => {
			return response.text()
		})
		.then((text_response) => {
			console.log(text_response)
			let object_response = JSON.parse(text_response)
			player_id = object_response.id
			let element = document.getElementById("pokemon_description")
			element.textContent = element.textContent.replace("???", object_response.description)
		})

	let pokemon_names_url;
	if (is_local()) {
		pokemon_names_url = new URL("http://localhost:1234/names")
	} else {
		pokemon_names_url = new URL("https://pokeguesser.baduit.eu/names")
	}
	pokemon_names_url.searchParams.append('lang', 'fr')
	fetch(pokemon_names_url, options)
		.then((response) => {
			return response.text()
		})
		.then((text_response) => {
			console.log(text_response)
			let object_response = JSON.parse(text_response)
			pokemon_names = object_response.names
			update_pokemon_list()
		})
}

startup()

