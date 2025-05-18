const express = require('express');
const mongoose = require('mongoose');
const jwt = require('jsonwebtoken');
const puppeteer = require('puppeteer');
const addon = require('../build/Release/cryptoAddon');
const app = express();
app.use(express.json());

mongoose.connect(process.env.MONGODB_URI || 'mongodb://localhost/scraper', {
  useNewUrlParser: true,
  useUnifiedTopology: true
});

const User = require('./models/Users.js');

// Middleware JWT
const authenticate = (req, res, next) => {
  const token = req.header('Authorization')?.replace('Bearer ', '');
  if (!token) return res.status(401).json({ error: 'Accès refusé' });
  try {
    const decoded = jwt.verify(token, 'secret');
    req.user = decoded;
    next();
  } catch (err) {
    res.status(401).json({ error: 'Token invalide' });
  }
};

// Endpoint de scraping
app.post('/scrape', authenticate, async (req, res) => {
  const { url } = req.body;
  try {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();
    await page.goto(url);
    const data = await page.evaluate(() => {
      const items = Array.from(document.querySelectorAll('.product')); // À adapter
      return items.map(item => ({
        name: item.querySelector('.name')?.textContent,
        price: item.querySelector('.price')?.textContent
      }));
    });
    await browser.close();

    // Chiffrer les données
    const key = 'ma_cle_secrete_32_caracteres_1234'; // À sécuriser
    const encrypted = addon.encrypt(JSON.stringify(data), key);
    res.json({ encrypted });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

app.post('/register', async (req, res) => {
  const { email, password } = req.body;
  const passwordHash = addon.hashPassword(password);
  const user = new User({ email, passwordHash });
  await user.save();
  res.status(201).json({ message: 'Utilisateur créé' });
});

app.post('/login', async (req, res) => {
  const { email, password } = req.body;
  const user = await User.findOne({ email });
  if (!user || !addon.verifyPassword(password, user.passwordHash)) {
    return res.status(401).json({ error: 'Identifiants invalides' });
  }
  const token = jwt.sign({ id: user._id }, 'secret', { expiresIn: '1h' });
  res.json({ token });
});

app.listen(3000, () => console.log('Serveur sur port 3000'));
