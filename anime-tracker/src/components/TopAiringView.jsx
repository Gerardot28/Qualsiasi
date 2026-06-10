import { useState } from 'react';
import { getTopAiring } from '../api/jikan';
import { useFetch } from '../hooks/useAnime';
import AnimeGrid from './AnimeGrid';
import styles from './TopAiringView.module.css';

export default function TopAiringView({ onCardClick }) {
  const [page, setPage] = useState(1);
  const { data, loading, error } = useFetch(() => getTopAiring(page), [page]);

  const items = data?.data || [];
  const lastPage = data?.pagination?.last_visible_page || 1;

  return (
    <div>
      <div className={styles.header}>
        <span className={styles.label}>🏆 Top Anime In Onda — per Punteggio</span>
      </div>

      <AnimeGrid
        items={items}
        loading={loading}
        error={error}
        onCardClick={onCardClick}
        emptyMsg="Nessun anime trovato."
      />

      {lastPage > 1 && !loading && (
        <div className={styles.pagination}>
          <button
            className={styles.pageBtn}
            disabled={page === 1}
            onClick={() => setPage((p) => p - 1)}
          >
            ← Precedente
          </button>
          <span className={styles.pageInfo}>
            Pagina {page} / {lastPage}
          </span>
          <button
            className={styles.pageBtn}
            disabled={page === lastPage}
            onClick={() => setPage((p) => p + 1)}
          >
            Successiva →
          </button>
        </div>
      )}
    </div>
  );
}
