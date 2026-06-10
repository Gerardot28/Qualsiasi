import { useState, useEffect } from 'react';
import { searchAnime } from '../api/jikan';
import AnimeGrid from './AnimeGrid';
import styles from './SearchView.module.css';

export default function SearchView({ query, onCardClick }) {
  const [page, setPage] = useState(1);
  const [data, setData] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (!query) return;
    setPage(1);
  }, [query]);

  useEffect(() => {
    if (!query) return;
    setLoading(true);
    setError(null);
    searchAnime(query, page)
      .then((d) => setData(d))
      .catch((e) => setError(e.message))
      .finally(() => setLoading(false));
  }, [query, page]);

  const items = data?.data || [];
  const lastPage = data?.pagination?.last_visible_page || 1;

  return (
    <div>
      <div className={styles.searchHeader}>
        <span className={styles.label}>
          Risultati per: <strong>"{query}"</strong>
        </span>
        {data?.pagination?.items?.total != null && (
          <span className={styles.count}>{data.pagination.items.total} risultati</span>
        )}
      </div>

      <AnimeGrid
        items={items}
        loading={loading}
        error={error}
        onCardClick={onCardClick}
        emptyMsg={`Nessun risultato per "${query}".`}
      />

      {lastPage > 1 && !loading && items.length > 0 && (
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
