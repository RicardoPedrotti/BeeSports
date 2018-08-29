SELECT * FROM `beesports`.`tb_login`;
SELECT * FROM  BEESPORTS.TB_ATLETA;

INSERT INTO `beesports`.`tb_pais`
(`CD_PAIS`,`NM_PAIS`)
VALUES(1,'Brasil');

INSERT INTO `beesports`.`tb_cidade`
(`CD_CIDADE`,`NM_CIDADE`,`CD_PAIS`,`DH_ULTIMA_ATU`)
VALUES (1,'São Paulo',1,NOW());

INSERT INTO `beesports`.`tb_endereco`
(`CD_ENDERECO`,`DS_ENDERECO`,`DS_ENDERECO2`,`CD_CEP`,`NM_BAIRRO`,`CD_CIDADE`,`DH_ULTIMA_ATU`)
VALUES (1,'Coronel Oscar Porto, 500','Apto. 172',04003002,'Paraíso',1,NOW());

INSERT INTO beesports.tb_atleta
(`CD_ATLETA`,`NM_ATLETA`,`DS_SEXO`,`NM_EMAIL`,`NM_PRIMEIRO_NOME`,`NM_ULTIMO_NOME`
,`DT_NASCIMENTO`,`CD_RG_ATLETA`,`CD_CPF_ATLETA`,`CD_ENDERECO`,`CD_RESPONSAVEL`,
`FOTO_ATLETA`,`DH_CRIACAO`,`DH_ULTIMA_ATU`,`NU_TELEFONE_RES`,`NU_TELEFONE_CEL`)
VALUES
(1,'Ricardo Pedrotti','M','rpedrottivf@gmail.com','Ricardo','Pedrotti',
'1996-11-11',359839836,41350500852,'1',NULL,NULL,
NOW(),NOW(),38874213,11987038900);



INSERT INTO beesports.tb_login (LOGIN, PASSWD, NM_USUARIO, NM_EMAIL, TP_USUARIO, CD_ATLETA, DH_ULTIMO_LOGIN)
VALUES ('teste', 'teste', 'Ricardo', 'rpedrottivf@gmail.com', 'A', 1, NOW());
