require('dotenv').config();

const express = require('express');
require('pg');
const handlebars = require('express-handlebars');
const path = require('path');

const app = express();

app.use(express.static(__dirname + '/views'));
app.engine('handlebars', handlebars.engine());
app.set('view engine', 'handlebars');
app.set('views', path.join(__dirname + '/views'));

const {
    POSTGRES_DATABASE,
    POSTGRES_USER,
    POSTGRES_PASSWORD,
    POSTGRES_HOST,
    POSTGRES_PORT,
} = process.env;

const Sequelize = require('sequelize');
const sequelize = new Sequelize({
    database: `${POSTGRES_DATABASE}`,
    username: `${POSTGRES_USER}`,
    password: `${POSTGRES_PASSWORD}`,
    host: `${POSTGRES_HOST}`,
    port: `${POSTGRES_PORT}`,
    dialect: 'postgres',
    dialectOptions: {
        ssl: {
            require: true,
            rejectUnauthorized: false,
        },
    },
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
        type: Sequelize.String,
    },
    umidade: {
        type: Sequelize.String,
    },
});

sensor.sync({force: true});

app.get('/', async function (req, res) {
    const sensors = await sensor.findAll({
        attributes: ['temperatura', 'umidade', 'createdAt'],
    });

    res.render(__dirname + '/views/home', { sensors });
});

app.get('/cadastrar/:temperatura/:umidade', async function (req, res) {
    console.log(req.params);
    await sensor.create({
        temperatura: req.params.temperatura,
        umidade: req.params.umidade,
    });
    res.redirect('/');
});

app.listen(8080, function () {
    console.log('Conexão do webserver Ok');
});
