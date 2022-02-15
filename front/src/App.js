import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import PokeGuesser from './PokeGuesser';
import { Col, Row } from 'react-bootstrap';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <Row>
          <Col>
            <img src='pokeball.png' className="App-logo" alt="logo" />
          </Col>
          <Col>
            PokéGuesser
          </Col>
        </Row>
      </header>
      <PokeGuesser />
    </div>
  );
}

export default App;
