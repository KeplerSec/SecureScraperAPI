# SecureScraperAPI
API Node.js pour scraper des sites web et chiffrer les données avec AES-256 via un addon C++ (N-API).

## Fonctionnalités
- **Scraping web** : Extraction de données (ex. : noms, prix) avec Puppeteer.
- **Chiffrement sécurisé** : Chiffrement AES-256 et hachage SHA-256 en C++.
- **Authentification** : Inscription/connexion avec JWT.
- **Stockage** : Données chiffrées dans MongoDB (Atlas/local).
- **Déploiement** : Prêt pour Render.

## Prérequis
- Node.js 22.x
- MongoDB (local ou Atlas)
- OpenSSL
- node-gyp
- Puppeteer (Chromium)

## Installation
```bash
npm install
node-gyp configure
node-gyp build
node src/server.js
