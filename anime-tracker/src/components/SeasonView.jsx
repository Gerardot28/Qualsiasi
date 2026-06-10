import { useState } from 'react';
import { getCurrentSeason } from '../api/jikan';
import { useFetch } from '../hooks/useAnime';
import AnimeGrid from './AnimeGrid';
import styles from './SeasonView.module.css';

const SEASON_NAMES = { winter: 'Inverno', spring: 'Primavera', summer: 'Estate', fall: 'Autunno' };

function getSeasonLabel(data) {
  if (!data?.length) return '';
  const first = data[0];
  const season = SEASON_NAMES[first.season] || first.season || '';
  const year = first.year || new Date().getFullYear();
  return `${season} ${year}`;
}

export default function SeasonView({ onCardClick }) {
  const [page, setPage] = useState(1);
  const { data, loading, error } = useFetch(() => getCurrentSeason(page), [page]);

  const items = data?.data || [];
  const lastPage = data?.pagination?.last_visible_page || 1;
  const seasonLabel = getSeasonLabel(items);

  return (
    <div>
      <div className={styles.seasonHeader}>
        <span className={styles.seasonBadge}>
          🌸 Stagione Corrente {seasonLabel && `— ${seasonLabel}`}
        </span>
        <span className={styles.count}>
          {data?.pagination?.items?.total
            ? `${data.pagination.items.total} anime`
            : ''}
        </span>
      </div>

      <AnimeGrid
        items={items}
        loading={loading}
        error={error}
        onCardClick={onCardClick}
        emptyMsg="Nessun anime trovato per questa stagione."
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
