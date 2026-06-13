import wikipedia
from requests.exceptions import JSONDecodeError

wikipedia.set_lang("pt")
wikipedia.set_user_agent("pln-script/1.0")


busca = wikipedia.search("Polo aquático")
page = wikipedia.page(busca[0])

print(page.summary)


esportes = ["Atletismo", "Badminton", "Basquete", "Basquete 3x3", "Beisebol Softbol", "Beisebol5", "Biatlo", "Bobsled", "Boxe", "Breaking", "Canoagem de Velocidade", "Canoagem Slalom", "Ciclismo BMX Freestyle", "Ciclismo BMX Racing", "Ciclismo de Estrada", "Ciclismo de Pista", "Ciclismo Mountain Bike", "Combinado nórdico", "Cricket", "Curling", "Escalada", "Esgrima", "Esqui alpino", "Esqui cross-country", "Esqui de Montanha", "Esqui estilo livre", "Flag Football", "Futebol", "Futsal", "Ginástica Acrobática", "Ginástica Artística", "Ginástica de Trampolim", "Ginástica Rítmica", "Golfe", "Handebol", "Handebol de Praia", "Hipismo", "Hóquei no gelo", "Hóquei sobre Grama", "Judô", "Karatê", "Lacrosse", "Levantamento de Peso", "Luge", "Luta", "Luta de praia", "Maratona Aquática", "Nado Artístico", "Natação", "Patinação artística", "Patinação de velocidade", "Patinação de velocidade em pista curta", "Patinação de Velocidade Inline", "Pentatlo Moderno", "Polo Aquático", "Remo", "Remo costeiro", "Rugby Sevens", "Salto de esqui", "Saltos Ornamentais", "Skate", "Skeleton", "Snowboard", "Squash", "Surfe", "Taekwondo", "Tênis", "Tênis de Mesa", "Tiro com Arco", "Tiro Esportivo", "Triatlo", "Vela", "Vôlei", "Vôlei de Praia", "Wushu"]