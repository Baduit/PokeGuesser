import React, { useEffect, useState } from 'react'
import { Container, Row, Col, Button, Form, Image, Table, Alert, Modal } from 'react-bootstrap'
import { Typeahead } from 'react-bootstrap-typeahead';

function PokeGuesser() {
  const [pokemonDescription, setPokemonDescription] = useState("En France, on mange 50 kilos de viande de boeuf chaque seconde, soit 1,55 million de tonnes de viande par an, dont 18% sont importés, malgré l'importance de l'élevage bovin en France. Cela représente 55 grammes de viande de boeuf par jour par personne en France.");
  const [pokemonName, setPokemonName] = useState("?");
  const [pokemonTypes, setPokemonTypes] = useState("?");
  const [pokemonWeight, setPokemonWeight] = useState("?");
  const [pokemonHeight, setPokemonHeight] = useState("?");
  const [pokemonImage, setPokemonImage] = useState("question-mark.png")
  const [playerID, setPlayerID] = useState('');
  const [pokemonGuess, setPokemonGuess] = useState([]);
  const [pokemonList, setPokemonList] = useState([]);
  const [show, setShow] = useState(false);
  const [step, setStep] = useState(1);
  const [error, setError] = useState('');

  const API = 'https://pokeguesser.baduit.eu/'

  useEffect(() => {
    const requestOptions = {
      method: 'GET',
      headers: { 'Content-Type': 'application/json' },
    };
    fetch(API + 'start', requestOptions)
      .then(response => response.json())
      .then(data => {
        setPokemonDescription(data.description);
        setPlayerID(data.id);
      });
    fetch(API + 'names?lang=fr', requestOptions)
      .then(response => response.json())
      .then(data => {
        setPokemonList(data.names);
      });
  }, []);

  const handleSubmit = (event) => {
    event.preventDefault();
    if (pokemonGuess.length > 0) {
      const requestOptions = {
        method: 'GET',
        headers: { 'Content-Type': 'application/json' },
      };
      let params = "?id=" + playerID + "&pokemon_name=" + pokemonGuess[0];
      fetch(API + 'guess' + params, requestOptions)
        .then(response => response.json())
        .then(data => {
          if (data.success === false) {
            setError('Raté ! Essaie encore.')
            switch (data.field_name) {
              case 'types':
                setPokemonTypes(data.field_value);
                setStep(step + 1);
                break;
              case 'height':
                setPokemonHeight(data.field_value);
                setStep(step + 1);
                break;
              case 'weight':
                setPokemonWeight(data.field_value);
                setStep(step + 1);
                break;
              case 'image_url':
                setPokemonImage(data.field_value);
                setStep(step + 1);
                break;
              case 'name':
                setPokemonName(data.field_value);
                setStep(step + 1);
                break;
              default:
                break;
            }
          } else {
            setShow(true);
            setError('');
            setPokemonTypes(data.pokemon.types);
            setPokemonHeight(data.pokemon.height);
            setPokemonWeight(data.pokemon.weight);
            setPokemonImage(data.pokemon.image_url);
            setPokemonName(data.pokemon.name);
          }
        });
    } else {
      setError("Ce pokémon n'existe pas !")
    }
  };

  const handleClose = () => {
    setShow(false);
  }

  return (
    <div className="PokeGuessr">
      <Container fluid>
        <Row>
          <Col>
            <Image
              src={pokemonImage}
              className='img-thumbnail'
            />
          </Col>
          <Col>
            <Table striped bordered hover size="sm" style={{ height: '100%', verticalAlign: 'middle' }}>
              <tbody>
                <tr>
                  <td>Nom</td>
                  <td>{pokemonName}</td>
                </tr>
                <tr>
                  <td>Types</td>
                  <td>{pokemonTypes.map((e) => ("[" + e + "] "))}</td>
                </tr>
                <tr>
                  <td>Poids</td>
                  <td>{pokemonWeight} {(pokemonWeight != '?' ? 'kg' : '')}</td>
                </tr>
                <tr>
                  <td>Taille</td>
                  <td>{pokemonHeight} {(pokemonHeight != '?' ? 'm' : '')}</td>
                </tr>
                <tr>
                  <td>Description</td>
                  <td>{pokemonDescription}</td>
                </tr>
              </tbody>
            </Table>
          </Col>
        </Row>
        <Row>
          <Form onSubmit={handleSubmit} className='d-flex justify-content-center' style={{ margin: '2vmin' }}>
            <Form.Group>
              <Typeahead
                id="basic-typeahead-single"
                labelKey="name"
                onChange={setPokemonGuess}
                options={pokemonList}
                placeholder="Nom du pokémon..."
                selected={pokemonGuess}
              />
            </Form.Group>
            <Button variant="primary" type="submit">
              Deviner
            </Button>
          </Form>
          {error !== '' ?
            <Alert key={0} variant={'danger'}>
              {error}
            </Alert>
            : <></>}
        </Row>
      </Container>

      <Modal show={show} onHide={handleClose}>
        <Modal.Header closeButton>
          <Modal.Title>Victoire !</Modal.Title>
        </Modal.Header>
        <Modal.Body>Félicitations, vous avez trouvé {pokemonName} en {step} essai{step > 1 ? 's' : ''} !</Modal.Body>
        <Modal.Footer>
          <Button variant="secondary" onClick={handleClose}>
            Fermer
          </Button>
        </Modal.Footer>
      </Modal>
    </div >
  );
}

export default PokeGuesser;
