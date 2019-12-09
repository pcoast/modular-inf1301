int LIS_verificador(LIS_tppCabecaLista pCabecaDaLista)
{

    int numFalhasObservadas, numNos, numSucessosObservados;

    LIS_tpNoLista *pNo, *ultimoNoDaVolta;

    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;

    CNT_InicializarContadores("EXEC/contagemacumulada.txt");
    CNT_IniciarContagem();
    CNT_LerContadores("EXEC/contadores.txt");

    numFalhasObservadas = 0;  
    numSucessosObservados = 0;

    if (!pCabecaDaLista->ExcluirValor)
   
    {
        ++numFalhasObservadas;
        CNT_Contar("13s", 0);
    }
    else
    {
        ++numSucessosObservados;
        CNT_Contar("13n", 0);
    }

    if (pCabecaDaLista->numNos && !pCabecaDaLista->pNoCorrente)
   
    {
        ++numFalhasObservadas;
        CNT_Contar("9s", 0);
    }
    else
    {
        ++numSucessosObservados;
        CNT_Contar("9n", 0);

        if (pCabecaDaLista->numNos && pCabecaDaLista->pNoCorrente->pCabeca != pCabecaDaLista)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("11s", 0);
        }
        else
        {
            ++numSucessosObservados;
            CNT_Contar("11n", 0);
        }
    }

    if (pCabecaDaLista->numNos && !pCabecaDaLista->pNoPrimeiro)
   
    {
        ++numFalhasObservadas;
        CNT_Contar("10s", 0);

        pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoUltimo;

        while (pCabecaDaLista->pNoPrimeiro->pNoAnterior)
            pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoPrimeiro->pNoAnterior;
    }
    else
    {
        ++numSucessosObservados;
        CNT_Contar("10n", 0);
        if (pCabecaDaLista->numNos && pCabecaDaLista->pNoPrimeiro->pCabeca != pCabecaDaLista)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("12s", 0);
            pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoUltimo;

            while (pCabecaDaLista->pNoPrimeiro->pNoAnterior)
                pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoPrimeiro->pNoAnterior;
        }
        else
        {
            ++numSucessosObservados;
            CNT_Contar("12n", 0);
        }
    }
   

    pNo = pCabecaDaLista->pNoPrimeiro;

    numNos = pCabecaDaLista->numNos;

    ultimoNoDaVolta = pCabecaDaLista->pNoPrimeiro;

    while (1)
    {

        if (!pNo)
            break;

        if (pCabecaDaLista->tipoEstrutura != pNo->tipoEstrutura)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("7s", 0);
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("7n", 0);
        }

        if (pCabecaDaLista->tamNo != pNo->tamNo)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("14s", 0);
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("14n", 0);
        }

        if (!pNo->pConteudo)
        {
            ++numFalhasObservadas;
            CNT_Contar("6s", 0);
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("6n", 0);
        }

        if (!pNo->pNoProximo && numNos == 1)
       
        {
            break;
        }

        if (!pNo->pNoProximo && numNos != 1)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("2s", 0);
            ultimoNoDaVolta = pNo;
            break;
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("2n", 0);
        }

        if (pNo->pNoProximo && numNos == 0)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("4s", 0);
            break;
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("4n", 0);
        }

        if (!pNo->pNoProximo->pNoAnterior)
       
        {
            ++numFalhasObservadas;
            CNT_Contar("3s", 0);
        }

        else
        {
            ++numSucessosObservados;
            CNT_Contar("3n", 0);

            if (pNo->pNoProximo->pNoAnterior != pNo)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("5s", 0);
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("5n", 0);
            }
        }

        pNo = pNo->pNoProximo;
        --numNos;
    }

    if (pNo != pCabecaDaLista->pNoUltimo)
    {

       
        pNo = pCabecaDaLista->pNoUltimo;

        numNos = pCabecaDaLista->numNos;

        while (pNo != ultimoNoDaVolta)
        {

            if (pCabecaDaLista->tipoEstrutura != pNo->tipoEstrutura)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("7s", 0);
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("7n", 0);
            }

            if (pCabecaDaLista->tamNo != pNo->tamNo)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("14s", 0);
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("14n", 0);
            }

            if (!pNo->pConteudo)
            {
                ++numFalhasObservadas;
                CNT_Contar("6s", 0);
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("6n", 0);
            }

            if (!pNo->pNoAnterior && numNos == 1)
           
            {
                break;
            }

            if (!pNo->pNoAnterior && numNos != 1)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("3s", 0);
                break;
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("3n", 0);
            }

            if (pNo->pNoAnterior && numNos == 0)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("5s", 0);
                break;
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("5n", 0);
            }

            if (pNo->pNoAnterior != ultimoNoDaVolta && !pNo->pNoAnterior->pNoProximo)
           
            {
                ++numFalhasObservadas;
                CNT_Contar("2s", 0);
            }

            else
            {
                ++numSucessosObservados;
                CNT_Contar("2n", 0);

                if (pNo->pNoAnterior != ultimoNoDaVolta && pNo->pNoAnterior->pNoProximo != pNo)
               
                {
                    ++numFalhasObservadas;
                    CNT_Contar("4s", 0);
                }

                else
                {
                    ++numSucessosObservados;
                    CNT_Contar("4n", 0);
                }
            }

            pNo = pNo->pNoAnterior;
            --numNos;
        }
    }

    CNT_GravarContadores("EXEC/contagemacumulada.txt");
    CNT_PararContagem();
    CNT_TerminarContadores();

    return numFalhasObservadas;
}