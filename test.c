#include "so_long.h"

char	*is_rectangle(int fd)
{
	int			size_line;
	char		*str;
	static char	*store;
	int			size_test;

	store = get_next_line(fd);
	size_line = ft_strlen(store);
	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		size_test = ft_strlen(str);
		store = ft_strjoin(store, str);
		if (size_test != size_line)
		{
			free(str);
			free(store);
			ft_printf("Error: Map is not rectangular\n");
			// Probleme si pas de retour a la ligne ou d'espace
			return (NULL);
		}
	}
	return (store);
}

int	is_enclosed(char **split, int i, int j)
{
	while (split[0][j])
	{
		if (split[0][j] != '1')
			return (ft_printf("Error: Map is not enclosed by walls\n"));
		j++;
	}
	while (split[i + 1])
	{
		if (split[i][0] != '1' || split[i][ft_strlen(split[i]) - 1] != '1')
			return (ft_printf("Error: Map is not enclosed by walls\n"));
		i++;
	}
	j = 0;
	while (split[i][j])
	{
		if (split[i][j] != '1')
			return (ft_printf("Error: Map is not enclosed by walls\n"));
		j++;
	}
	return (0);
}

int	condition_check(char *store)
{
	int	i;
	int	count_e;
	int	count_p;
	int	count_c;

	count_e = 0;
	count_p = 0;
	count_c = 0;
	i = 0;
	while (store[i])
	{
		if (store[i] == 'E')
			count_e++;
		if (store[i] == 'P')
			count_p++;
		if (store[i] == 'C')
			count_c++;
		i++;
	}
	if (count_c < 1 || count_e != 1 || count_p != 1)
	{
		ft_printf("Error: Map elements (E, P, C) are incorrect\n");
		return (1);
	}
	return (0);
}

void    explore(char **map, int x, int y, t_access *access)
{
    if (map[y][x] == '1' || map[y][x] == 'V') // Mur ou case visitée
        return;
    if (map[y][x] == 'C') // Collectible trouvé
        access->collectibles--;
    if (map[y][x] == 'E') // Sortie trouvée
        access->exit_found = 1;
    map[y][x] = 'V'; // Marque la case comme visitée

    // Appels récursifs pour les cases adjacentes
    explore(map, x + 1, y, access); // Droite
    explore(map, x - 1, y, access); // Gauche
    explore(map, x, y + 1, access); // Bas
    explore(map, x, y - 1, access); // Haut
}

int is_map_accessible(char **map, int player_x, int player_y, int collectibles_count) // J ai besoin d avoir count_c + la position P
{
    t_access access = { collectibles_count, 0 };
    explore(map, player_x, player_y, &access);

    // Vérifie si tous les collectibles et la sortie ont été atteints
    return (access.collectibles == 0 && access.exit_found);
}

int free_and_close(char *store, int fd)
{
    free (store);
    //free (map);
    close (fd);
    return (1);
}
int	main(void)
{
	int		fd;
	char	*store;
    char    **map;

	fd = open("map.ber", O_RDONLY);
	if (fd == -1)
		return (ft_printf("Map doesn't exist\n"));
	store = is_rectangle(fd);
	if (!store)
		return (free_and_close(store, fd)); // penser a free la map aussi
    map = ft_split(store, '\n');
    if (!map)
        return (free_and_close(store, fd));
	if (is_enclosed(map, 1, 0))
		return (free_and_close(store, fd));
	if (condition_check(store))
		return (free_and_close(store, fd));
	ft_printf("Map is valid\n");
	free(store);
	close(fd);
	return (0);
}

