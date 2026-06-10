import AnimeCard from './AnimeCard';
import styles from './AnimeGrid.module.css';

export default function AnimeGrid({ items, onCardClick, loading, error, emptyMsg }) {
  if (loading) {
    return (
      <div className={styles.center}>
        <div className={styles.spinner} />
        <p className={styles.loadingText}>Caricamento in corso...</p>
      </div>
    );
  }

  if (error) {
    return (
      <div className={styles.center}>
        <p className={styles.error}>⚠ {error}</p>
      </div>
    );
  }

  if (!items?.length) {
    return (
      <div className={styles.center}>
        <p className={styles.empty}>{emptyMsg || 'Nessun risultato trovato.'}</p>
      </div>
    );
  }

  return (
    <div className={styles.grid}>
      {items.map((anime) => (
        <AnimeCard key={anime.mal_id} anime={anime} onClick={onCardClick} />
      ))}
    </div>
  );
}
