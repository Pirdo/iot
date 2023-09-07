const express = require('express');
const app = express();

const Sequelize = require('sequelize');
const sequelize = new Sequelize('projetoesp', 'pires', 'pirdo', {
    hoot: '34.27.20.114',
    port: '5432',
    dialect: 'postgres',
});

sequelize
    .authenticate()
    .then(function () {
        console.log('Conexão com o banco de dados realizada com sucesso.');
    })
    .catch(function (erro) {
        console.log('Erro na conexão com o banco de dados: ' + erro);
    });

const sensor = sequelize.define('sensor', {
    temperatura: {
        type: Sequelize.INTEGER,
    },
    umidade: {
        type: Sequelize.INTEGER,
    },
});

sensor.sync({ force: true });

app.get('/', function (req, res) {
    res.send('Projeto ESP');
});

app.get('/cadastrar/:temperatura/:umidade', function (req, res) {
    console.log(req.params);
    sensor.create({
        temperatura: req.params.temperatura,
        umidade: req.params.umidade,
    });
    res.redirect('/');
});

app.listen(8081, function () {
    console.log('Conexão do webserver Ok');
});
