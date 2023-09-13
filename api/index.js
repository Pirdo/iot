require('dotenv').config();
const express = require('express');

const handlebars = require('express-handlebars');

const app = express();

app.use(express.static('views'));
app.engine('handlebars', handlebars.engine());
app.set('view engine', 'handlebars');
app.set('views', './api/views');

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
        type: Sequelize.INTEGER,
    },
    umidade: {
        type: Sequelize.INTEGER,
    },
});

sensor.sync();

app.get('/', async function (req, res) {
    const sensors = await sensor.findAll({
        attributes: ['temperatura', 'umidade', 'createdAt'],
    });
    res.setHeader('Content-Type', 'text/html');
    res.setHeader('Cache-Control', 's-max-age=1, stale-while-revalidate');

    res.render('home', { sensors });
});

app.get('/cadastrar/:temperatura/:umidade', function (req, res) {
    console.log(req.params);
    sensor.create({
        temperatura: req.params.temperatura,
        umidade: req.params.umidade,
    });
    res.redirect('/');
});

app.listen(8080, function () {
    console.log('Conexão do webserver Ok');
});
