let player_id = 0

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

function update_height(height) {
	let element = document.getElementById("pokemon_height")
	element.textContent = element.textContent.replace("???", height.toString())
}

function update_name(name) {
	let element = document.getElementById("pokemon_name")
	element.textContent = element.textContent.replace("???", name)
}

function try_guess_pokemon() {
	const pokemon_name = document.getElementById("title_input").value
	if (pokemon_name == "") {
		return
	}

	const options = {
		method: 'GET'
	}

	let url = new URL("http://localhost:1234/guess")
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

function startup() {
	const options = {
		method: 'GET'
	}

	let url = "http://localhost:1234/start"

	fetch(url, options)
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
}

startup()

