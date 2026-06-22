# pip install spacy
# python -m spacy download pt_core_news_sm
# pip install wikipedia

import wikipedia
from requests.exceptions import JSONDecodeError
import spacy


wikipedia.set_lang("pt")
wikipedia.set_user_agent("pln-script/1.0")

# Carregar o modelo em português
nlp = spacy.load("pt_core_news_sm")

esportes = ["Atletismo", "Badminton", "Basquete", "Basquete 3x3", "Beisebol Softbol", "Beisebol5", "Biatlo", "Bobsled", "Boxe", "Breaking", "Canoagem de Velocidade", "Canoagem Slalom", "Ciclismo BMX Freestyle", "Ciclismo BMX Racing", "Ciclismo de Estrada", "Ciclismo de Pista", "Ciclismo Mountain Bike", "Combinado nórdico", "Cricket", "Curling", "Escalada", "Esgrima", "Esqui alpino", "Esqui cross-country", "Esqui de Montanha", "Esqui estilo livre", "Flag Football", "Futebol", "Futsal", "Ginástica Acrobática", "Ginástica Artística", "Ginástica de Trampolim", "Ginástica Rítmica", "Golfe", "Handebol", "Handebol de Praia", "Hipismo", "Hóquei no gelo", "Hóquei sobre Grama", "Judô", "Karatê", "Lacrosse", "Levantamento de Peso", "Luge", "Luta", "Luta de praia", "Maratona Aquática", "Nado Artístico", "Natação", "Patinação artística", "Patinação de velocidade", "Patinação de velocidade em pista curta", "Patinação de Velocidade Inline", "Pentatlo Moderno", "Polo Aquático", "Remo", "Remo costeiro", "Rugby Sevens", "Salto de esqui", "Saltos Ornamentais", "Skate", "Skeleton", "Snowboard", "Squash", "Surfe", "Taekwondo", "Tênis", "Tênis de Mesa", "Tiro com Arco", "Tiro Esportivo", "Triatlo", "Vela", "Vôlei", "Vôlei de Praia", "Wushu"]


palavra_e_vertices_extraidos = []
for esporte in esportes:

    busca = wikipedia.search(esporte)
    page = wikipedia.page(busca[0])

    texto = page.summary
    
    # Processar o texto pelo pipeline do spaCy
    documento = nlp(texto)
    
    # Estruturas para armazenar os vértices extraídos
    vertices_extraidos = []
    
    #print(f"{'TEXTO ORIGINAL':<15} | {'POS (CLASSE)':<10} | {'LEMA (RAIZ)':<15}")
    #print("-" * 45)
    
    # 5. Iterar sobre cada palavra (token) do documento
    for token in documento:
        # Ignorar pontuação e espaços em branco
        if token.is_punct or token.is_space:
            continue
            
        # Exibir a análise de cada palavra
        #print(f"{token.text:<15} | {token.pos_:<10} | {token.lemma_:<15}")
        
        # 6. REGRA DE FILTRAGEM: Manter apenas Substantivos (NOUN), Verbos (VERB) e Nomes Próprios (PROPN)
        if token.pos_ in ["NOUN", "VERB", "PROPN"]:
            # Utilizamos o lema para evitar duplicatas no grafo (ex: 'chutou' vira 'chutar')
            vertices_extraidos.append(token.lemma_)
    
    print("\n" + "="*45)
    print("VÉRTICES FINAIS PARA O GRAFO:")
    print([esporte,vertices_extraidos])
    palavra_e_vertices_extraidos.append([esporte, vertices_extraidos])
print(palavra_e_vertices_extraidos)