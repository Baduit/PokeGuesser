from typing import Tuple, List, Dict
from dataclasses import dataclass, field
from serde import serde
from serde.json import from_json
from os import listdir
from random import choice


@serde
@dataclass
class Pokemon:
    name: str
    descriptions: list[str]
    types: list[str]
    height: int
    weight: int
    image_url: str


@serde
@dataclass
class Metadata:
    names: List[str]


# Hard coded because I'm lazy
POKE_DATA_PATH = "./poke_data/generated_data"


def get_random_pokemon(lang: str) -> Pokemon:
    poke_dir = f"{POKE_DATA_PATH}/{lang}/pokeguesser"
    poke_files = listdir(poke_dir)
    choosen_poke_file = choice(poke_files)
    with open(f"{poke_dir}/{choosen_poke_file}") as f:
        serialized_pokemon = f.read()
    return from_json(Pokemon, serialized_pokemon)


@dataclass
class NamesHandler:
    names: Dict[str, List[str]] = field(default_factory=dict)

    def get_all_pokemon_names(self, lang: str) -> List[str]:
        if not lang in self.names.keys():
            self._load_all_pokemon_names(lang)
        return self.names[lang]

    def _load_all_pokemon_names(self, lang: str):
        with open(f"{POKE_DATA_PATH}/{lang}/metadata.json") as f:
            serialized_metadata = f.read()
        metadata = from_json(Metadata, serialized_metadata)
        self.names[lang] = metadata.names

