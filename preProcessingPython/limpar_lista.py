import json

# Supondo que 'dados' receba o array do JSON original que você enviou
with open('webscraper padronizado.json', 'r', encoding='utf-8') as f:
    dados = json.load(f)

dados_limpos = []

for item in dados:
    esporte = item["esporte"]
    # Transforma tudo em minúscula, joga no Set para remover duplicadas e volta para lista ordenada
    palavras_limpas = sorted(list(set([palavra.lower() for palavra in item["descricao"]])))
    
    dados_limpos.append({
        "esporte": esporte,
        "descricao": palavras_limpas
    })

# Exportando o resultado limpo
with open('dados_limpos.json', 'w', encoding='utf-8') as f:
    json.dump(dados_limpos, f, ensure_ascii=False, indent=2)

print("Dados limpos com sucesso!")