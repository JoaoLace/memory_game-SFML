import os
import sys
import requests
from duckduckgo_search import DDGS

pasta = "assets/random"

entrada = " ".join(sys.argv[1:]) if len(sys.argv) > 1 else ""
if not entrada:
    print("Nenhum argumento recebido")
    sys.exit(1)

print(f"Recebi: {entrada}")
os.makedirs(pasta, exist_ok=True)

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"
}

with DDGS(headers=headers) as ddgs:
    try:
        results = ddgs.images(entrada, max_results=10)
        for i, result in enumerate(results, 1):
            image_url = result["image"]
            try:
                response = requests.get(image_url, headers=headers, timeout=10)
                if response.status_code == 200:
                    nome_arquivo = os.path.join(pasta, f"{i}.jpg")
                    with open(nome_arquivo, "wb") as f:
                        f.write(response.content)
                    print(f"Imagem salva como {nome_arquivo}")
                else:
                    print(f"Erro ao baixar imagem: código {response.status_code}")
            except Exception as e:
                print(f"Erro ao baixar {image_url}: {e}")
    except Exception as e:
        print(f"Erro na busca por imagens: {e}")
