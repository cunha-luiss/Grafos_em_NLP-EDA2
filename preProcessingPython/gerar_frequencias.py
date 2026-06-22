import json
import unicodedata

def normalize(w):
    w = w.lower().strip()
    w = "".join(c for c in unicodedata.normalize('NFD', w) if unicodedata.category(c) != 'Mn')
    return w

def main():
    with open('data/webscraper_padronizado.json', 'r', encoding='utf-8') as f:
        padronizado = json.load(f)

    with open('webscraper bruto.json', 'r', encoding='utf-8') as f:
        bruto = json.load(f)

    bruto_dict = {}
    for item in bruto:
        bruto_dict[item['esporte']] = item['descricao']

    novos_dados = []
    missing_sports = []
    
    for item in padronizado:
        esporte = item['esporte']
        desc_padrao = item['descricao']
        
        if esporte not in bruto_dict:
            missing_sports.append(esporte)
            novos_dados.append(item)
            continue
            
        raw_words = bruto_dict[esporte]
        
        word_freqs = {normalize(w): 0 for w in desc_padrao}
        
        for raw_w in raw_words:
            norm_raw = normalize(raw_w)
            if norm_raw in word_freqs:
                word_freqs[norm_raw] += 1
                
        for norm_w in word_freqs:
            if word_freqs[norm_w] == 0:
                word_freqs[norm_w] = 1
                
        nova_descricao = []
        for word in sorted(desc_padrao):
            norm_w = normalize(word)
            freq = word_freqs[norm_w]
            nova_descricao.extend([word] * freq)
            
        novos_dados.append({
            "esporte": esporte,
            "descricao": nova_descricao
        })

    if missing_sports:
        print(f"Alerta: os seguintes esportes não foram achados no bruto e usaram frequência 1: {missing_sports}")

    with open('data/webscraper_padronizado.json', 'w', encoding='utf-8') as f:
        json.dump(novos_dados, f, ensure_ascii=False, indent=2)

    print("Dados de frequencia gerados com sucesso!")

if __name__ == '__main__':
    main()
