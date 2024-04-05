use std::io::{self, BufRead};

fn main() {
    let mut games: Vec<Vec<u32>> = Vec::new();

    // Ler input
    {
        let input_lock = io::stdin();
        let input_iter = input_lock.lock().lines();

        for line in input_iter {
            let game: Vec<u32> = line.expect("Failed to read a line")
                .split_whitespace()
                .map(|s| s.parse().expect("One of the elements is not a number"))
                .collect();

            // Testar fim do input
            if game.len() == 1 && game[0] == 0 {
                break;
            }
            
            // Adicionar linha ao vetor de jogos
            games.push(game);
        }
    }

    // Verificar quem ganhou em cada jogo
    for mut game in games {
        // Ignorar o primeiro item, que é o número de elementos
        let permutation = &mut game[1..];
        let mut swaps: u32 = 0;

        // Shell sort
        let mut gap = permutation.len() / 2;
        while gap > 0 {
            for i in gap..permutation.len() {
                let mut j = i;
                while j >= gap && permutation[j - gap] > permutation[j] {
                    permutation.swap(j, j - gap);
                    swaps += 1;
                    j -= gap;
                }
            }

            gap /= 2;
        }

        if swaps % 2 == 0 {
            println!("Carlos");
        } else {
            println!("Marcelo");
        }
    }
}
