import json
import unicodedata
from pathlib import Path


CAMINHO_JSON = Path("data/webscraper_padronizado.json")


def normalizar(texto):
    texto = texto.lower()
    return "".join(
        caractere
        for caractere in unicodedata.normalize("NFD", texto)
        if unicodedata.category(caractere) != "Mn"
    )


def carregar_palavras_existentes(dados):
    palavras = {}

    for item in dados:
        for palavra in item["descricao"]:
            palavras.setdefault(normalizar(palavra), palavra)

    return palavras


def main():
    dados = json.loads(CAMINHO_JSON.read_text(encoding="utf-8"))
    palavra_existente = carregar_palavras_existentes(dados)

    tags_por_esporte = {
        "atletismo": ["corrida", "pista", "resistencia"],
        "triatlo": ["corrida", "resistencia", "ciclismo", "natacao"],
        "pentatlo moderno": [
            "corrida",
            "esgrima",
            "lamina",
            "arma",
            "natacao",
            "tiro",
            "hipismo",
        ],
        "maratona aquatica": ["natacao", "agua", "resistencia"],
        "natacao": ["natacao", "agua", "piscina"],
        "nado artistico": ["natacao", "agua", "piscina"],
        "beisebol softbol": ["bola", "campo", "rebatedor", "taco"],
        "beisebol5": ["bola", "campo", "rebatedor", "beisebol"],
        "cricket": ["bola", "campo", "rebatedor", "taco"],
        "golfe": ["bola", "campo", "taco"],
        "flag football": ["campo", "futebol", "touchdown", "bola oval"],
        "rugby sevens": ["campo", "bola oval"],
        "futebol": ["campo", "futebol", "gol"],
        "futsal": ["futebol", "gol", "quadra"],
        "ginastica de trampolim": ["ginastica", "salto", "acrobacia", "aparelho"],
        "ginastica acrobatica": ["ginastica", "salto", "acrobacia"],
        "ginastica artistica": ["ginastica", "salto", "aparelho", "acrobacia"],
        "ginastica ritmica": ["ginastica", "aparelho", "musica"],
        "saltos ornamentais": ["salto", "acrobacia", "agua"],
        "esgrima": ["esgrima", "lamina", "arma"],
        "remo costeiro": ["remo", "barco", "agua"],
        "remo": ["remo", "barco", "agua"],
        "vela": ["barco", "agua"],
    }

    for item in dados:
        descricao = item["descricao"]
        tags = tags_por_esporte.get(normalizar(item["esporte"]), [])

        for tag in tags:
            palavra = palavra_existente.get(normalizar(tag), tag)
            if palavra not in descricao:
                descricao.append(palavra)

    CAMINHO_JSON.write_text(
        json.dumps(dados, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
