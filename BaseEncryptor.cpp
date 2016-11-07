#include "BaseEncryptor.h"

Encryptor::Encryptor(const EVP_CIPHER *type, ContentProvider *cpIn, ContentProvider *cpOut, ContentProvider *cpKey) {
    this->type = type;
    this->setCtx(cpIn, cpOut, cpKey);
}

Encryptor::~Encryptor() { delete this->_in; delete this->_out; delete this->_key; delete this->type;}

void Encryptor::processKey(bool generateKey) {
    auto keyCP = this->getKeyCP();
    if (generateKey) {
        std::vector<u_char> key;
        if (!this->generateKey(key)) throw 500;
        keyCP->write(key);
        keyCP->init();
    } else {
        if (!this->validateKey(keyCP)) throw 400;
    }
}

bool Encryptor::validateKey(ContentProvider *keyToSet) {
    return this->checkLengthOfKey(keyToSet->size(false));
}

ContentProvider* Encryptor::getInCP() {return this->_in;}
ContentProvider* Encryptor::getOutCP() {return this->_out;}
ContentProvider* Encryptor::getKeyCP() {return this->_key;}
const EVP_CIPHER* Encryptor::getType() {return this->type;}

void Encryptor::setCtx(ContentProvider *cpIn, ContentProvider *cpOut, ContentProvider *cpKey) {
    if (cpIn != NULL) this->_in = cpIn;
    if (cpOut != NULL) this->_out = cpOut;
    if (cpKey != NULL) this->_key = cpKey;
}