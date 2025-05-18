const addon = require('../build/Release/cryptoAddon');

describe('Crypto Addon', () => {
  test('should hash password correctly', () => {
    const hash = addon.hashPassword('test');
    expect(hash).toMatch(/^[0-9a-f]{64}$/);
  });

  test('should encrypt data', () => {
    const data = 'test data';
    const key = 'ma_cle_secrete_32_caracteres_1234';
    const encrypted = addon.encrypt(data, key);
    expect(encrypted).toMatch(/^[0-9a-f]+$/);
  });
});